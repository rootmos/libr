#pragma once

#ifdef UV_VERSION_MAJOR
#define CHECK_UV(err, format, ...) do { \
    if(err != 0) { \
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 0, \
                   format ": %s\n", ##__VA_ARGS__, uv_err_name(err)); \
    } \
} while(0)
#endif
