#include "util.h"
#include "logging.h"

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

API void LIBR(failwith0)(
    const char* const caller,
    const char* const file,
    const unsigned int line,
    const int include_errno,
    const char* const fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);

    if(include_errno) {
        LIBR(logger)(LOG_ERROR, caller, file, line, "(%s) ", strerror(errno));
        if(vdprintf(LIBR(logger_fd), fmt, vl) < 0) {
            abort();
        }
    } else {
        LIBR(vlogger)(LOG_ERROR, caller, file, line, fmt, vl);
    }
    va_end(vl);

    abort();
}
