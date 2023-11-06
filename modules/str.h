#pragma once

#include <sys/types.h>

ssize_t endswith(const char* str, const char* suffix);

#ifdef failwith
long long parse_bytes(const char* str, const char** err);
#endif
