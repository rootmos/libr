#include "sha.h"
#include "util.h"

#include <assert.h>
#include <endian.h>
#include <stdlib.h>
#include <string.h>

struct sha1_state_internal {
    uint32_t H[5];

    uint64_t len;

    uint8_t buf[512/8];
    uint8_t off;
};

static struct sha1_state_internal* sha1_to_internal_state(struct sha1_state* st)
{
    assert(sizeof(struct sha1_state) == sizeof(struct sha1_state_internal));
    return (struct sha1_state_internal*)st;
}

static uint32_t sha1_f(int t, uint32_t B, uint32_t C, uint32_t D)
{
    if(0 <= t && t <= 19) {
        return (B & C) | ((~B) & D);
    } else if(20 <= t && t <= 39) {
        return B ^ C ^ D;
    } else if(40 <= t && t <= 59) {
        return (B & C) | (B & D) | (C & D);
    } else if(60 <= t && t <= 79) {
        return B ^ C ^ D;
    } else {
        abort();
    }
}

static uint32_t sha1_K(int t)
{
    if(0 <= t && t <= 19) {
        return 0x5A827999;
    } else if(20 <= t && t <= 39) {
        return 0x6ED9EBA1;
    } else if(40 <= t && t <= 59) {
        return 0x8F1BBCDC;
    } else if(60 <= t && t <= 79) {
        return 0xCA62C1D6;
    } else {
        abort();
    }
}

static uint32_t sha1_S(uint32_t X, int n)
{
    return (X << n) | (X >> (32-n));
}

void sha1_init(struct sha1_state* st_)
{
    struct sha1_state_internal* st = sha1_to_internal_state(st_);

    st->H[0] = 0x67452301;
    st->H[1] = 0xEFCDAB89;
    st->H[2] = 0x98BADCFE;
    st->H[3] = 0x10325476;
    st->H[4] = 0xC3D2E1F0;

    st->len = 0;
    st->off = 0;
}

static void sha1_process_block(struct sha1_state_internal* st, uint32_t block[16])
{
    uint32_t W[80];
    for(size_t i = 0; i < 16; i++) {
        W[i] = be32toh(block[i]);
    }

    for(size_t t = 16; t < 80; t++) {
        W[t] = sha1_S(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16], 1);
    }

    uint32_t A = st->H[0], B = st->H[1], C = st->H[2], D = st->H[3], E = st->H[4];

    for(size_t t = 0; t < 80; t++) {
        uint32_t T = sha1_S(A, 5) + sha1_f(t, B, C, D) + E + W[t] + sha1_K(t);
        E = D;
        D = C;
        C = sha1_S(B, 30);
        B = A;
        A = T;
    }

    st->H[0] += A;
    st->H[1] += B;
    st->H[2] += C;
    st->H[3] += D;
    st->H[4] += E;
}

void sha1_update(struct sha1_state* st_, void* buf, size_t len)
{
    struct sha1_state_internal* st = sha1_to_internal_state(st_);

    uint8_t* b = buf;
    size_t l = len;

    while(l > 0) {
        size_t L = MIN(l, sizeof(st->buf) - st->off);

        memcpy(&st->buf[st->off], b, L);
        b += L;
        l -= L;
        st->off += L;

        if(sizeof(st->buf) == st->off) {
            sha1_process_block(st, (uint32_t*)st->buf);
            st->off = 0;
        }
    }

    st->len += len;
}

void sha1_finalize(struct sha1_state* st_)
{
    struct sha1_state_internal* st = sha1_to_internal_state(st_);

    assert(st->len*8 <= 0xffffffffffffffff);

    size_t k = 0;
    for(; ((st->len*8 + 1 + k) % 512) != 448; k++);
    size_t l = (k+1)/8;
    uint8_t buf[l + 8];
    memset(buf, 0, sizeof(buf));

    *((uint64_t*)(&buf[l])) = htobe64(st->len*8);
    buf[0] |= 0x80;

    sha1_update(st_, LIT(buf));

    for(size_t i = 0; i < 5; i++) {
        st->H[i] = htobe32(st->H[i]);
    }
}
