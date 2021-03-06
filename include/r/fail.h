#pragma once

#define CHECK(res, format, ...) CHECK_NOT(res, -1, format, ##__VA_ARGS__)

#define CHECK_NOT(res, err, format, ...) \
    CHECK_IF(res == err, format, ##__VA_ARGS__)

#define CHECK_IF(cond, format, ...) do { \
    if(cond) { \
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 1, \
                   format "\n", ##__VA_ARGS__); \
    } \
} while(0)

#define CHECK_MALLOC(x) CHECK_NOT(x, NULL, "memory allocation failed")

#ifdef SND_LIB_VERSION
#define CHECK_ALSA(err, format, ...) do { \
    if(err < 0) { \
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 0, \
                   format ": %s\n", ##__VA_ARGS__, snd_strerror(err)); \
    } \
} while(0)
#endif

#ifdef CL_SUCCESS
#define CHECK_OCL(err, format, ...) do { \
    if(err != CL_SUCCESS) { \
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 0, \
                   format ": %d\n", ##__VA_ARGS__, err); \
    } \
} while(0)
#endif

#ifdef FREETYPE_MAJOR
#define CHECK_FT(err, format, ...) do { \
    if(err != FT_Err_Ok) { \
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 0, \
                   format ": (%d) %s\n", ##__VA_ARGS__, err, FT_Error_String(err)); \
    } \
} while(0)
#endif

#ifdef VK_HEADER_VERSION
#define CHECK_VULKAN(res, format, ...) do { \
    if(res != VK_SUCCESS) { \
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 0, \
                   format ": %d\n", ##__VA_ARGS__, res); \
    } \
} while(0)
#endif

#ifdef AVERROR
#define CHECK_AV(res, format, ...) do { \
    if(res < 0) { \
        r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
                   __extension__ __LINE__, 0, \
                   format ": %s\n", ##__VA_ARGS__, av_err2str(res)); \
    } \
} while(0)
#endif

#define failwith(format, ...) \
    r_failwith(__extension__ __FUNCTION__, __extension__ __FILE__, \
               __extension__ __LINE__, 0, format "\n", ##__VA_ARGS__)

#define not_implemented() failwith("not implemented")

void r_failwith(const char* const caller,
                const char* const file,
                const unsigned int line,
                const int include_errno,
                const char* const fmt, ...)
    __attribute__ ((noreturn, format (printf, 5, 6)));
