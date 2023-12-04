#pragma once

#include <stdint.h>
#include <stddef.h>

#define SHA1_DIGEST_LENGTH (160/8)

struct sha1_state {
    uint8_t digest[SHA1_DIGEST_LENGTH];
    uint8_t _internal[84];
};

void LIBR(sha1_init)(struct sha1_state* st);
void LIBR(sha1_update)(struct sha1_state* st, const void* buf, size_t len);
void LIBR(sha1_finalize)(struct sha1_state* st);
