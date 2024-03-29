#pragma once

#include <stddef.h>
#include <stdint.h>

void LIBR(random_bytes)(void* buf, size_t n);

static inline uint64_t LIBR(xorshift64)(uint64_t* state)
{
    uint64_t x = *state;
    x^= x << 13;
    x^= x >> 7;
    x^= x << 17;
    *state = x;
    return x;
}

static inline uint64_t LIBR(xorshift128plus)(uint64_t* state)
{
    uint64_t x = state[0];
    uint64_t const y = state[1];
    state[0] = y;
    x ^= x << 23; // a
    state[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
    return state[1] + y;
}

extern uint64_t LIBR(xorshift_state)[2];
void LIBR(xorshift_state_initialize)(void);
inline static uint64_t LIBR(xorshift64_i)(void) { return LIBR(xorshift64)(LIBR(xorshift_state)); }
inline static uint64_t LIBR(xorshift128plus_i)(void) { return LIBR(xorshift128plus)(LIBR(xorshift_state)); }

/* f = uniform_float(x) => 0 <= f < 1 */
inline static float LIBR(uniform_float)(uint64_t x)
{
    return (float)x/UINT64_MAX;
}

float LIBR(normal_dist)(uint64_t* s);
inline static float LIBR(normal_dist_i)(void) { return LIBR(normal_dist)(LIBR(xorshift_state)); }
