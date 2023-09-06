#pragma once

#include <stddef.h>
#include <sys/types.h>

size_t path_join(char* buf, size_t L, const char* p0, ...);

int makedirs(const char* path, mode_t mode);
