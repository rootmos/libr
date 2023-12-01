#include "xorshift.h"
#include "fail.h"

#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#ifndef ENTROPY_SOURCE
#define ENTROPY_SOURCE "/dev/urandom"
#endif

API void LIBR(random_bytes)(void* buf, size_t n)
{
    int fd = open(ENTROPY_SOURCE, O_RDONLY);
    CHECK(fd, "open("ENTROPY_SOURCE")");

    uint8_t* p = buf;
    size_t i = 0;
    while(i < n) {
        ssize_t r = read(fd, p, n-i);
        CHECK(r, "read("ENTROPY_SOURCE", %zu", n);
        i += r;
        p += r;
    }

    int s = close(fd); CHECK(s, "close("ENTROPY_SOURCE")");
}

uint64_t LIBR(xorshift_state)[2] API;
API void LIBR(xorshift_state_initialize)(void)
{
    LIBR(random_bytes)(LIBR(xorshift_state), sizeof(LIBR(xorshift_state)));
}

API float LIBR(normal_dist)(uint64_t* s)
{
    float x, y, r2;
    do {
        x = 2*LIBR(uniform_float)(LIBR(xorshift64)(s)) - 1;
        y = 2*LIBR(uniform_float)(LIBR(xorshift64)(s)) - 1;
        r2 = x*x + y*y;
    } while(r2 > 1 || r2 == 0);

    return x * sqrtf(-2*logf(r2)/r2);
}
