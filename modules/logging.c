#include "logging.h"
#include "now.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

API void LIBR(dummy)(int foo, ...)
{
    abort();
}

int LIBR(logger_fd) API = 2;

API void LIBR(vlogger)(
    int level,
    const char* const caller,
    const char* const file,
    const unsigned int line,
    const char* const fmt, va_list vl)
{
    int r = dprintf(LIBR(logger_fd), "%s:%d:%s:%s:%u ",
        LIBR(now_iso8601_compact)(), getpid(), caller, file, line);
    if(r < 0) {
        abort();
    }

    r = vdprintf(LIBR(logger_fd), fmt, vl);
    if(r < 0) {
        abort();
    }
}

API void LIBR(logger)(
    int level,
    const char* const caller,
    const char* const file,
    const unsigned int line,
    const char* const fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    LIBR(vlogger)(level, caller, file, line, fmt, vl);
    va_end(vl);
}
