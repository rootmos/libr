#include "devnull.h"

#include <fcntl.h>
#include <unistd.h>

API int LIBR(devnull)(int flags)
{
    int fd = open("/dev/null", flags);
    CHECK(fd, "open(/dev/null, %d)", flags);
    return fd;
}

API void LIBR(devnull2)(int fd, int flags)
{
    int null = LIBR(devnull)(flags);
    int r = dup2(null, fd); CHECK(r, "dup2(.., %d)", fd);
    r = close(null); CHECK(r, "close");
}
