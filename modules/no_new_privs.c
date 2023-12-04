#include "no_new_privs.h"
#include "fail.h"

#include <sys/prctl.h>

API void LIBR(no_new_privs)(void)
{
    int r = prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
    CHECK(r, "prctl(PR_SET_NO_NEW_PRIVS, 1)");
}
