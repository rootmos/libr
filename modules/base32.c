#include "base32.h"

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

ssize_t base32_decode(void* out, const char* data, size_t l)
{
    size_t bs = 0;
    for(size_t i = 0; l && *data && *data != '='; l -= i, data += i, i = 0) {
        uint64_t g = 0;
        for(; i < MIN(l, 8); i++) {
            uint64_t c = data[i];
            if(c >= 'A' && c <= 'Z') g |= (c - 'A') << (59-i*5);
            else if(c >= '2' && c <= '7') g |= (c - '2' + 26) << (59-i*5);
            else if(c == '=') break;
            else return -1;
        }
        g = htobe64(g);
        memcpy(out + bs, &g, i*5/8);
        bs += i*5/8;
    }
    return bs;
}
