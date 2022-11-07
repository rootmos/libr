#include "xorshift.h"
#include "fail.h"

#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#ifndef ENTROPY_SOURCE
#define ENTROPY_SOURCE "/dev/urandom"
#endif

void random_bytes(void* buf, size_t n)
{
    int fd = open(ENTROPY_SOURCE, O_RDONLY);
    CHECK(fd, "open("ENTROPY_SOURCE")");
    ssize_t r = read(fd, buf, n); CHECK(r, "read("ENTROPY_SOURCE", %zu", n);
    assert(r == n);
    int s = close(fd); CHECK(s, "close("ENTROPY_SOURCE")");
}

uint64_t xorshift_state[2];
void xorshift_state_initialize(void)
{
    random_bytes(xorshift_state, sizeof(xorshift_state));
}

float normal_dist(uint64_t* s)
{
    float x, y, r2;
    do {
        x = 2*uniform_float(xorshift64(s)) - 1;
        y = 2*uniform_float(xorshift64(s)) - 1;
        r2 = x*x + y*y;
    } while(r2 > 1 || r2 == 0);

    return x * sqrtf(-2*logf(r2)/r2);
}
