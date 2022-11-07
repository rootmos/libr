#pragma once

#include <stddef.h>

struct line {
    char* str;
    size_t len;
};

struct file {
    struct line* lines;
    size_t n_lines;
};

struct file* read_file(const char* fn);
void free_file(struct file* f);
