#include "logging.h"
#include "util.h"
#include "fail.h"

#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
void r_dummy(...)
#else
void r_dummy()
#endif
{
    failwith("called the dummy function, you dummy!");
}

void r_vlog(int level,
            const char* const caller,
            const char* const file,
            const unsigned int line,
            const char* const fmt, va_list vl)
{
    fprintf(stderr, "%s:%d:%s:%s:%u ",
            now_iso8601(), getpid(), caller, file, line);

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
