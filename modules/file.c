#include "file.h"
#include "fail.h"
#include "logging.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#ifndef FILE_MAX_LINE_LENGTH
#define FILE_MAX_LINE_LENGTH 1024
#endif

#ifndef FILE_MAX_LINES
#define FILE_MAX_LINES 1024
#endif

struct lines* read_lines(const char* fn)
{
    int fd = open(fn, O_RDONLY);
    if(fd == -1 && errno == ENOENT) {
#ifdef FILE_READ_LINES_ENOENT_NULL
        debug("file does not exist: %s", fn);
        return NULL;
#else
        failwith("file does not exist: %s", fn);
#endif
    }
    CHECK(fd, "open(%s)", fn);
    debug("reading file: %s", fn);

    struct line lines[FILE_MAX_LINES];
    ssize_t n_lines = 0;

    char buf[FILE_MAX_LINE_LENGTH];
    size_t l = 0;
    while(1) {
        ssize_t s = read(fd, &buf[l], 1);
        if(s == 1) {
            if(buf[l] == '\n') {
                buf[l] = 0;
                trace("read line %zu: %s", n_lines, buf);

                lines[n_lines].str = strndup(buf, l);
                CHECK_MALLOC(lines[n_lines].str);
                lines[n_lines].len = l;

                l = 0;
                n_lines += 1;
            } else {
                l += 1;
            }
        } else {
            break;
        }
    }

    int r = close(fd); CHECK(r, "close");

    struct lines* f = calloc(sizeof(struct lines), 1);
    CHECK_MALLOC(f);
    f->n_lines = n_lines;

    f->lines = calloc(sizeof(struct line), n_lines);
    CHECK_MALLOC(f->lines);
    memcpy(f->lines, lines, sizeof(struct line)*n_lines);

    return f;
}

void free_lines(struct lines* f)
{
    if(f == NULL) {
        return;
    }

    free(f->lines);
    free(f);
}

struct wholefile* read_wholefile(const char* fn)
{
    int fd = open(fn, O_RDONLY);
    if(fd == -1 && errno == ENOENT) {
#ifdef FILE_READ_WHOLEFILE_ENOENT_NULL
        debug("file does not exist: %s", fn);
        return NULL;
#else
        failwith("file does not exist: %s", fn);
#endif
    }

    struct stat st;
    int r = fstat(fd, &st);
    CHECK(r, "fstat(%s)", fn);
    const size_t l = st.st_size;

    void* b = mmap(NULL, l, PROT_READ, MAP_PRIVATE, fd, 0); CHECK_MMAP(b);

    struct wholefile* f = malloc(sizeof(struct wholefile)); CHECK_MALLOC(f);
    f->len = l;
    f->buf = malloc(l); CHECK_MALLOC(f->buf);
    memcpy(f->buf, b, l);

    return f;
}

void free_wholefile(struct wholefile* f)
{
    if(f == NULL) {
        return;
    }

    free(f->buf);
    free(f);
}

void writefile(const char* fn, void* buf, size_t len)
{
    int fd = open(fn, O_WRONLY);
    CHECK(fd, "open(%s, O_WRONLY)", fn);
    debug("writing file: %s", fn);

    writefile2(fd, buf, len);

    int r = close(fd); CHECK(r, "close");
}

void writefile2(int fd, void* buf, size_t len)
{
    size_t i = 0;
    while(i < len) {
        ssize_t s = write(fd, buf+i, len-i);
        CHECK(s, "write(%d)", fd);
        trace("wrote: %zd bytes", s);
        i += s;
    }
}
