#pragma once

#include <stddef.h>
#include <stdint.h>

void random_bytes(void* buf, size_t n);

static inline uint64_t xorshift64(uint64_t* state)
{
    uint64_t x = *state;
    x^= x << 13;
    x^= x >> 7;
    x^= x << 17;
    *state = x;
    return x;
}

static inline uint64_t xorshift128plus(uint64_t* state)
{
    uint64_t x = state[0];
    uint64_t const y = state[1];
    state[0] = y;
    x ^= x << 23; // a
    state[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
    return state[1] + y;
}

extern uint64_t xorshift_state[2];
void xorshift_state_initalize(void);
inline static uint64_t xorshift64_i(void) { return xorshift64(xorshift_state); }
inline static uint64_t xorshift128plus_i(void) { return xorshift128plus(xorshift_state); }

/* f = uniform_float(x) => 0 <= f < 1 */
inline static float uniform_float(uint64_t x)
{
    return (float)x/UINT64_MAX;
}

float normal_dist(uint64_t* s);
inline static float normal_dist_i(void) { return normal_dist(xorshift_state); }
