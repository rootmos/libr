#pragma once

#include <stdint.h>
#include <stddef.h>

#define SHA256_DIGEST_LENGTH (256/8)

struct sha256_state {
    uint8_t digest[SHA256_DIGEST_LENGTH];
    uint8_t _internal[80];
};

void sha256_init(struct sha256_state* st);
void sha256_update(struct sha256_state* st, const void* buf, size_t len);
void sha256_finalize(struct sha256_state* st);
