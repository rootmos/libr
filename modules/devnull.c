#include "devnull.h"

#include <fcntl.h>
#include <unistd.h>

int devnull(int flags)
{
    flags |= O_CLOEXEC;
    int fd = open("/dev/null", flags);
    CHECK(fd, "open(/dev/null, %d)", flags);
    return fd;
}

void devnull2(int fd, int flags)
{
    int null = devnull(flags);
    int r = dup2(null, fd); CHECK(r, "dup2(.., %d)", fd);
    r = close(null); CHECK(r, "close");
}
