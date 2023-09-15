#include "sha.h"

#include <assert.h>

struct sha1_state_internal {
    uint8_t digest[SHA1_DIGEST_LENGTH];
};

static struct sha1_state_internal* sha1_to_internal_state(struct sha1_state* st)
{
    assert(sizeof(struct sha1_state) == sizeof(struct sha1_state_internal));
    return (struct sha1_state_internal*)st;
}

void sha1_init(struct sha1_state* st_)
{
    struct sha1_state_internal* st = sha1_to_internal_state(st_);
    assert(st);
}

void sha1_update(struct sha1_state* st, void* buf, size_t len)
{
}

void sha1_finalize(struct sha1_state* st)
{
}
