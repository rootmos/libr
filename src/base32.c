#include <r.h>

#include <string.h>
#include <endian.h>

size_t base32_encode(char* out, const void* data, size_t l)
{
    const char* const start = out;

    for(; l > 0; l -= MIN(l, 5), data += 5) {
        size_t L = 8;

        switch(l) {
        case 1: L = 2; break;
        case 2: L = 4; break;
        case 3: L = 5; break;
        case 4: L = 7; break;
        }

        uint64_t buf = 0;
        memcpy(&buf, data, MIN(l, 5));
        buf = be64toh(buf);

        for(uint64_t m = 0xf800000000000000, i = 1; i <= 8; i++, m >>= 5) {
            *out++ = i <= L
                ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"[(buf & m) >> (64 - 5*i)]
                : '=';
        }
    }

    return *out++ = 0, out - start;
}
