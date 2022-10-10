#pragma once

int landlock_abi_version(void);
int landlock_new_ruleset(void);
void landlock_allow_read_file(int fd, const char* path);
void landlock_apply(int fd);
