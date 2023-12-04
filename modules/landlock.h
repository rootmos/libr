#pragma once

#include <linux/types.h>

int LIBR(landlock_abi_version)(void);
int LIBR(landlock_new_ruleset)(void);
void LIBR(landlock_allow)(int rsfd, const char* path, __u64 allowed_access);
void LIBR(landlock_allow_read)(int fd, const char* path);
void LIBR(landlock_allow_read_write)(int rsfd, const char* path);
void LIBR(landlock_apply)(int fd);
