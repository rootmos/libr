#include "path.h"

#include <string.h>
#include <sys/stat.h>
#include <errno.h>

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

int makedirs(const char* path, mode_t mode)
{
    const size_t L = strlen(path);
    char buf[L+1];
    memcpy(buf, path, L+1);

    for(size_t l = (buf[0] == '/' ? 1 : 0); l < L; l++) {
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
    CHECK(r, "stat(%s)", buf);

    if(!(st.st_mode & S_IFDIR)) {
        errno = EEXIST;
        return -1;
    }

    return 0;
}
