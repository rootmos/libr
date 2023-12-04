#pragma once

#include <stddef.h>

struct line {
    char* str;
    size_t len;
};

struct lines {
    struct line* lines;
    size_t n_lines;
};

struct lines* LIBR(read_lines)(const char* fn);
void LIBR(free_lines)(struct lines* f);

struct wholefile {
    void* buf;
    size_t len;
};

struct wholefile* LIBR(read_wholefile)(const char* fn);
void LIBR(free_wholefile)(struct wholefile* f);

void LIBR(writefile)(const char* fn, void* buf, size_t len);
void LIBR(writefile2)(int fd, void* buf, size_t len); // assumes fd is not O_NONBLOCK
