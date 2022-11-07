#pragma once

#define CHECK_LUA(L, err, format, ...) do { \
    if(err != LUA_OK) { \
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 0, \
                   format ": %s\n", ##__VA_ARGS__, lua_tostring(L, -1)); \
    } \
} while(0)

#define LUA_EXPECT_TYPE(L, t, expected, format, ...) do { \
    if(t != expected) {\
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 0, \
                   format ": unexpected type %s (expected %s)\n", \
                   ##__VA_ARGS__, lua_typename(L, t), \
                   lua_typename(L, expected)); \
    } \
} while(0)

#ifndef LUA_STACK_NEUTRAL_TERM
#define LUA_STACK_NEUTRAL_TERM __lua_stack_top
#endif

#define lua_stack_neutral_begin(L) int LUA_STACK_NEUTRAL_TERM = lua_gettop(L)
#define lua_stack_neutral_end(L) \
    CHECK_IF(LUA_STACK_NEUTRAL_TERM != lua_gettop(L), \
             "redundant stack elements present")
