#include "path.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

size_t path_joinv(char* buf, size_t L, const char* p0, va_list ps)
{
    size_t n = strlen(p0);
    if(n < L) {
        memmove(buf, p0, n);
    }

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

    return n;
}

size_t path_join(char* buf, size_t L, const char* p0, ...)
{
    va_list ps; va_start(ps, p0);
    size_t n = path_joinv(buf, L, p0, ps);
    return va_end(ps), n;
}

#ifdef failwith
const char* path_joinvs(const char* p0, va_list ps)
{
    static char buf[PATH_MAX];
    size_t l = path_joinv(buf, sizeof(buf), p0, ps);
    if(l >= sizeof(buf)) {
        failwith("buffer overflow");
    }

    return buf;
}

const char* path_joins(const char* p0, ...)
{
    va_list ps; va_start(ps, p0);
    const char* p = path_joinvs(p0, ps);
    return va_end(ps), p;
}
#endif

int makedirs(const char* path, mode_t mode)
{
    const size_t L = strlen(path);
    char buf[L+1];
    memcpy(buf, path, L+1);

    for(size_t l = (buf[0] == '/' ? 1 : 0); l <= L; l++) {
        if(buf[l] == '/') {
            buf[l] = 0;
        }

        if(buf[l] == 0) {
            int r = mkdir(buf, mode);
            if(r == -1) {
                if(errno != EEXIST) {
                    return -1;
                }
            }

            if(l < L) {
                buf[l] = '/';
            }
        }
    }

    struct stat st;
    int r = stat(buf, &st);
    if(r != 0) return -1;

    if(!(st.st_mode & S_IFDIR)) {
        errno = EEXIST;
        return -1;
    }

    return 0;
}
