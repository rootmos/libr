#include "seccomp.h"

#include <sys/syscall.h>
#include <unistd.h>

API int LIBR(seccomp)(unsigned int operation, unsigned int flags, void *args)
{
    return syscall(SYS_seccomp, operation, flags, args);
}
