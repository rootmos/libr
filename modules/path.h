#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

size_t path_join(char* buf, size_t L, const char* p0, ...);
size_t vpath_join(char* buf, size_t L, const char* p0, va_list ps);

int makedirs(const char* path, mode_t mode);
