#include "now.h"

#include <time.h>
#include <stdlib.h>

PRIVATE const char* LIBR(now_iso8601_compact)(void)
{
    static char buf[17];
    const time_t t = time(NULL);
    size_t r = strftime(buf, sizeof(buf), "%Y%m%dT%H%M%SZ", gmtime(&t));
    if(r <= 0) abort();
    return buf;
}
