#include "sha2.h"

#include <assert.h>
#include <endian.h>
#include <string.h>

struct sha256_state_internal {
    uint32_t H[8];

    uint64_t len;

    uint8_t buf[512/8];
    uint8_t off;
};

static struct sha256_state_internal* sha256_to_internal_state(struct sha256_state* st)
{
    assert(sizeof(struct sha256_state) == sizeof(struct sha256_state_internal));
    return (struct sha256_state_internal*)st;
}

static uint32_t sha2_ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ ((~x) & z);
}

static uint32_t sha2_maj(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

static uint32_t sha2_rotr32(uint32_t X, int n)
{
    return (X >> n) | (X << (32-n));
}

static uint32_t sha2_bsig0(uint32_t x)
{
    return sha2_rotr32(x, 2) ^ sha2_rotr32(x, 13) ^ sha2_rotr32(x, 22);
}

static uint32_t sha2_bsig1(uint32_t x)
{
    return sha2_rotr32(x, 6) ^ sha2_rotr32(x, 11) ^ sha2_rotr32(x, 25);
}

static uint32_t sha2_ssig0(uint32_t x)
{
    return sha2_rotr32(x, 7) ^ sha2_rotr32(x, 18) ^ (x >> 3);
}

static uint32_t sha2_ssig1(uint32_t x)
{
    return sha2_rotr32(x, 17) ^ sha2_rotr32(x, 19) ^ (x >> 10);
}

static const uint32_t sha2_K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

void sha256_init(struct sha256_state* st_)
{
    struct sha256_state_internal* st = sha256_to_internal_state(st_);

    st->H[0] = 0x6a09e667;
    st->H[1] = 0xbb67ae85;
    st->H[2] = 0x3c6ef372;
    st->H[3] = 0xa54ff53a;
    st->H[4] = 0x510e527f;
    st->H[5] = 0x9b05688c;
    st->H[6] = 0x1f83d9ab;
    st->H[7] = 0x5be0cd19;

    st->len = 0;
    st->off = 0;
}

static void sha256_process_block(struct sha256_state_internal* st, uint32_t block[16])
{
    uint32_t W[64];
    for(size_t t = 0; t < 16; t++) {
        W[t] = be32toh(block[t]);
    }

    for(size_t t = 16; t < 64; t++) {
        W[t] = sha2_ssig1(W[t-2]) + W[t-7] + sha2_ssig0(W[t-15]) + W[t-16];
    }

    uint32_t a = st->H[0];
    uint32_t b = st->H[1];
    uint32_t c = st->H[2];
    uint32_t d = st->H[3];
    uint32_t e = st->H[4];
    uint32_t f = st->H[5];
    uint32_t g = st->H[6];
    uint32_t h = st->H[7];

    for(size_t t = 0; t < 64; t++) {
        uint32_t T1 = h + sha2_bsig1(e) + sha2_ch(e, f, g) + sha2_K[t] + W[t];
        uint32_t T2 = sha2_bsig0(a) + sha2_maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    st->H[0] += a;
    st->H[1] += b;
    st->H[2] += c;
    st->H[3] += d;
    st->H[4] += e;
    st->H[5] += f;
    st->H[6] += g;
    st->H[7] += h;
}

void sha256_update(struct sha256_state* st_, const void* buf, size_t len)
{
    struct sha256_state_internal* st = sha256_to_internal_state(st_);

    const uint8_t* b = buf;
    size_t l = len;

    while(l > 0) {
        size_t L = MIN(l, sizeof(st->buf) - st->off);

        memcpy(&st->buf[st->off], b, L);
        b += L;
        l -= L;
        st->off += L;

        if(sizeof(st->buf) == st->off) {
            sha256_process_block(st, (uint32_t*)st->buf);
            st->off = 0;
        }
    }

    st->len += len;
}

void sha256_finalize(struct sha256_state* st_)
{
    struct sha256_state_internal* st = sha256_to_internal_state(st_);

    assert(st->len*8 <= 0xffffffffffffffff);

    size_t k = 0;
    for(; ((st->len*8 + 1 + k) % 512) != 448; k++);
    size_t l = (k+1)/8;
    uint8_t buf[l + 8];
    memset(buf, 0, sizeof(buf));

    *((uint64_t*)(&buf[l])) = htobe64(st->len*8);
    buf[0] |= 0x80;

    sha256_update(st_, LIT(buf));

    for(size_t i = 0; i < 8; i++) {
        st->H[i] = htobe32(st->H[i]);
    }
}

