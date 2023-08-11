#include "fail.h"
#include "util.h"
#include "lua.h"
#include "now.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lua.h>

void r_lua_failwith(lua_State* L,
        const char* const caller,
        const char* const file,
        const unsigned int line,
        const char* const fmt, ...)
{

    size_t N = 1;
    char* buf;
    while(1) {
        buf = alloca(N);

        int r = snprintf(buf, N, "(%s:%d:%s:%s:%u) ", now_iso8601_compact(), getpid(), caller, file, line);
        if(r >= N) {
            while(N < r) {
                N <<= 1;
            }
            continue;
        }

        va_list vl;
        va_start(vl, fmt);

        r += vsnprintf(buf+r, N-r, fmt, vl);
        if(r < N) {
            lua_pushstring(L, buf);
            lua_error(L); // "This function does a long jump, and therefore never returns [...]."
        }

        va_end(vl);
        N <<= 1;
    }
}
