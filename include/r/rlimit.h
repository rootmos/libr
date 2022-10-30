#pragma once

#include <stddef.h>

enum rlimit_action {
    RLIMIT_ACTION_INHERIT = 0,
    RLIMIT_ACTION_ZERO = 1,
    RLIMIT_ACTION_ABS = 2,
    RLIMIT_ACTION_EQUAL = 3,
};

struct rlimit_spec {
    const char* name;
    int resource;
    enum rlimit_action action;
    unsigned long value;
};

void rlimit_default(struct rlimit_spec rlimits[], size_t len);
void rlimit_inherit(struct rlimit_spec rlimits[], size_t len);

int rlimit_parse(struct rlimit_spec rlimits[], size_t len, const char* str);
void rlimit_apply(const struct rlimit_spec rlimits[], size_t len);
