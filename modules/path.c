#include "path.h"

#include <string.h>

size_t path_join(char* buf, size_t L, const char* p0, ...)
{
    size_t n = strlen(p0);
    if(n < L) {
        memcpy(buf, p0, n);
    }

    va_list ps;
    va_start(ps, p0);
    for(;;) {
        const char* p = va_arg(ps, const char*);
        if(!p) break;

        if(n < L) {
            buf[n] = '/';
        }
        n += 1;

        size_t l = strlen(p);
        if(n + l < L) {
            memcpy(&buf[n], p, l);
        }
        n += l;
    }

    if(n < L) {
        buf[n] = 0;
    }

    va_end(ps);

    return n;
}
