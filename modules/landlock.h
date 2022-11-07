#pragma once

#include <linux/types.h>

int landlock_abi_version(void);
int landlock_new_ruleset(void);
void landlock_allow(int rsfd, const char* path, __u64 allowed_access);
void landlock_allow_read(int fd, const char* path);
void landlock_allow_read_write(int rsfd, const char* path);
void landlock_apply(int fd);
