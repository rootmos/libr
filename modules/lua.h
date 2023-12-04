#pragma once

#include <lua.h>

#define CHECK_LUA(L, err, format, ...) do { \
    if(err != LUA_OK) { \
        LIBR(failwith0)( \
            __extension__ __FUNCTION__, __extension__ __FILE__, \
            __extension__ __LINE__, 0, \
            format ": %s\n", ##__VA_ARGS__, lua_tostring(L, -1)); \
    } \
} while(0)

#define LUA_EXPECT_TYPE(L, t, expected, format, ...) do { \
    if(t != expected) { \
        LIBR(failwith0)( \
            __extension__ __FUNCTION__, __extension__ __FILE__, \
            __extension__ __LINE__, 0, \
            format ": unexpected type %s (expected %s)\n", \
            ##__VA_ARGS__, lua_typename(L, t), \
            lua_typename(L, expected)); \
    } \
} while(0)

#ifndef LUA_STACK_MARKER
#define LUA_STACK_MARKER LIBR(luaR_stack_marker)
#endif

#define luaR_stack(L) int LUA_STACK_MARKER = lua_gettop(L)
#define luaR_stack_expect(L, n) do { \
    int r = lua_gettop(L) - LUA_STACK_MARKER; \
    if(r < n) { \
        failwith("too few stack elements: found %d expected %d", r ,n); \
    } else if(r > n) { \
        failwith("too many stack elements: found %d expected %d", r ,n); \
    } \
} while(0)

#define luaR_return(L, n) do { \
    luaR_stack_expect(L, n); \
    return n; \
} while(0)

#define luaR_failwith(L, format, ...) \
    LIBR(luaR_failwith0)(L, \
            __extension__ __FUNCTION__, \
            __extension__ __FILE__, \
            __extension__ __LINE__, \
            format, ##__VA_ARGS__)

void LIBR(luaR_failwith0)(
    lua_State* L,
    const char* const caller,
    const char* const file,
    const unsigned int line,
    const char* const fmt, ...)
__attribute__ ((noreturn, format (printf, 5, 6)));

int LIBR(luaR_testmetatable)(lua_State* L, int arg, const char* tname);
void LIBR(luaR_checkmetatable)(lua_State* L, int arg, const char* tname);
