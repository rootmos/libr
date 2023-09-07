#pragma once

#include <signal.h>

#define TEST_SUITE(name, body) int main(int argc, char* argv[]) \
{ \
    struct test_runner_opts test_runner_opts; \
    test_runner_parse_options(&test_runner_opts, argc, argv); \
    test_runner_opts.test_suite = #name; \
    body \
    return 0; \
} \

#define TEST(f, body) { void f(void) { body } if(test_runner(&test_runner_opts, #f, f, -1, 0) != 0) exit(1); } while(0)
#define TEST_ABORT(f, body) { void f(void) { body } if(test_runner(&test_runner_opts, #f, f, SIGABRT, -1) != 0) exit(1); } while(0)

struct test_runner_opts {
    const char* test_suite;
    int propagate_stdout;
    int propagate_stderr;
};

void test_runner_parse_options(struct test_runner_opts* opts, int argc, char* argv[]);

int test_runner(struct test_runner_opts* opts,
                const char* name,
                void (*body)(void),
                int expect_signal,
                int expect_exit_code);
