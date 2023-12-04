#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

size_t LIBR(path_join)(char* buf, size_t L, const char* p0, ...);
size_t LIBR(path_joinv)(char* buf, size_t L, const char* p0, va_list ps);

// using static buffer
#ifdef failwith
const char* LIBR(path_joins)(const char* p0, ...);
const char* LIBR(path_joinvs)(const char* p0, va_list ps);
#endif

int LIBR(makedirs)(const char* path, mode_t mode);
