#include "logging.h"
#include "now.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef __cplusplus
void r_dummy(...)
#else
void r_dummy()
#endif
{
    abort();
}

void r_vlog(int level,
            const char* const caller,
            const char* const file,
            const unsigned int line,
            const char* const fmt, va_list vl)
{
    fprintf(stderr, "%s:%d:%s:%s:%u ",
            LIBR(now_iso8601_compact)(), getpid(), caller, file, line);

    vfprintf(stderr, fmt, vl);
}

void r_log(int level,
           const char* const caller,
           const char* const file,
           const unsigned int line,
           const char* const fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    r_vlog(level, caller, file, line, fmt, vl);
    va_end(vl);
}
