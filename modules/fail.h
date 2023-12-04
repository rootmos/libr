#pragma once

#define CHECK(res, format, ...) CHECK_NOT(res, -1, format, ##__VA_ARGS__)

#define CHECK_NOT(res, err, format, ...) \
    CHECK_IF(res == err, format, ##__VA_ARGS__)

#define CHECK_IF(cond, format, ...) do { \
    if(cond) { \
        LIBR(failwith0)(__extension__ __FUNCTION__, __extension__ __FILE__, \
            __extension__ __LINE__, 1, \
            format "\n", ##__VA_ARGS__); \
    } \
} while(0)

#define CHECK_MALLOC(x) CHECK_NOT(x, NULL, "memory allocation failed")
#define CHECK_MMAP(x) CHECK_NOT(x, MAP_FAILED, "memory mapping failed")

#define failwith(format, ...) \
    LIBR(failwith0)(__extension__ __FUNCTION__, __extension__ __FILE__, \
        __extension__ __LINE__, 0, format "\n", ##__VA_ARGS__)

#define not_implemented() LIBR(failwith0)("not implemented")

void LIBR(failwith0)(
    const char* const caller,
    const char* const file,
    const unsigned int line,
    const int include_errno,
    const char* const fmt, ...)
__attribute__ ((noreturn, format (printf, 5, 6)));
