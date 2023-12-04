#include "fail.h"
#include "util.h"
#include "lua.h"
#include "now.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lua.h>
#include <lauxlib.h>

API void LIBR(luaR_failwith0)(
    lua_State* L,
    const char* const caller,
    const char* const file,
    const unsigned int line,
    const char* const fmt, ...)
{

    size_t N = 256;
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

API int LIBR(luaR_testmetatable)(lua_State* L, int arg, const char* tname)
{
    if(lua_getmetatable(L, arg)) {
        luaL_getmetatable(L, tname);
        int r = lua_rawequal(L, -1, -2);
        lua_pop(L, 2);
        return r;
    }
    return 0;
}

API void LIBR(luaR_checkmetatable)(lua_State* L, int arg, const char* tname)
{
    luaL_argexpected(L, LIBR(luaR_testmetatable)(L, arg, tname), arg, tname);
}
