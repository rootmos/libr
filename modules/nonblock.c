#include "nonblock.h"
#include "fail.h"

#include <fcntl.h>

API void LIBR(set_blocking)(int fd, int blocking)
{
    int fl = fcntl(fd, F_GETFL, 0);
    if(blocking) {
        fl &= ~O_NONBLOCK;
    } else {
        fl |= O_NONBLOCK;
    }

    int r = fcntl(fd, F_SETFL, fl);
    CHECK(r, "fcntl(%d, F_SETFL, %d)", fd, fl);
}
