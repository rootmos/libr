#pragma once

#include <sys/types.h>

ssize_t LIBR(endswith)(const char* str, const char* suffix);

long long LIBR(parse_bytes)(const char* str, const char** err);
ssize_t LIBR(render_bytes)(char* buf, size_t len, size_t bs);
