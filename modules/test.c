#include "test.h"
#include "now.h"
#include "fail.h"
#include "util.h"

#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void dump_captured_output(int from_fd, const char* suffix,
                                 const char* name, const char* when)
{
    char fn[1024];
    ssize_t s = snprintf(fn, sizeof(fn), "test.%s.%s.%s", when, name, suffix);
    CHECK_IF(s < 0, "snprintf");
    if(s >= sizeof(fn)) { failwith("fn buffer too small"); }

    int out = open(fn, O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    CHECK(out, "open(%s)", fn);
    int r; off_t o = 0;
    do {
        r = splice(from_fd, NULL, out, &o, 1024, 0); CHECK(r, "splice");
    } while(r > 0);
    r = close(out); CHECK(r, "close");
}

API void LIBR(test_runner_parse_options)(struct test_runner_opts* o, int argc, char* argv[])
{
    memset(o, 0, sizeof(*o));

    int res;
    while((res = getopt(argc, argv, "oe")) != -1) {
        switch(res) {
        case 'o':
            o->propagate_stdout = 1;
            break;
        case 'e':
            o->propagate_stderr = 1;
            break;
        default:
            exit(1);
        }
    }
}

API int LIBR(test_runner)(
    struct test_runner_opts* opts,
    const char* name,
    void (*body)(void),
    int expect_signal,
    int expect_exit_code)
{
    char fullname[1024];
    int r = snprintf(LIT(fullname), "%s.%s", opts->test_suite, name);
    if(r >= sizeof(fullname)) {
        failwith("buffer overflow");
    }

    const char* now = LIBR(now_iso8601_compact)();

    int captured_stdout[2]; r = pipe(captured_stdout); CHECK(r, "pipe");
    int captured_stderr[2]; r = pipe(captured_stderr); CHECK(r, "pipe");

    pid_t pid = fork(); CHECK(pid, "fork()");
    if(pid == 0) {
        if(opts->propagate_stdout) {
            r = dup2(1, captured_stdout[1]); CHECK(r, "dup2");
        } else {
            r = close(captured_stdout[0]); CHECK(r, "close");
            r = dup2(captured_stdout[1], 1); CHECK(r, "dup2");
        }

        if(opts->propagate_stderr) {
            r = dup2(2, captured_stderr[1]); CHECK(r, "dup2");
        } else {
            r = close(captured_stderr[0]); CHECK(r, "close");
            r = dup2(captured_stderr[1], 2); CHECK(r, "dup2");
        }

        body();
        exit(0);
    }

    r = close(captured_stdout[1]); CHECK(r, "close");
    r = close(captured_stderr[1]); CHECK(r, "close");

    dprintf(2, "%s %s ", now, fullname);

    int res = -1;

    siginfo_t si;
    pid_t w = waitid(P_PID, pid, &si, WEXITED | WSTOPPED); CHECK(w, "waitid");
    if(expect_signal >= 0) {
        if(si.si_code != CLD_KILLED && si.si_code != CLD_DUMPED) {
            if(si.si_code == CLD_EXITED) {
                dprintf(2, "FAIL (exit=%d)\n", si.si_status);
            } else {
                failwith("unexpected si.si_code=%d", si.si_code);
            }
        } else if(si.si_status != expect_signal) {
            dprintf(2, "FAIL (signal=%s)\n", strsignal(si.si_status));
        } else {
            dprintf(2, "OK\n");
            res = 0;
        }
    } else if(expect_exit_code >= 0) {
        if(si.si_code != CLD_EXITED) {
            if(si.si_code == CLD_KILLED || si.si_code == CLD_DUMPED) {
                dprintf(2, "FAIL (signal=%s)\n", strsignal(si.si_status));
            } else {
                failwith("unexpected si.si_code=%d", si.si_code);
            }
        } else if(si.si_status != expect_exit_code ) {
            dprintf(2, "FAIL (exit=%d)\n", si.si_status);
        } else {
            dprintf(2, "OK\n");
            res = 0;
        }
    }

    dump_captured_output(captured_stdout[0], "stdout", fullname, now);
    dump_captured_output(captured_stderr[0], "stderr", fullname, now);

    r = close(captured_stdout[0]); CHECK(r, "close");
    r = close(captured_stderr[0]); CHECK(r, "close");

    return res;
}
