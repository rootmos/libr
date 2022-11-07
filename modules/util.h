#pragma once

#define LENGTH(xs) (sizeof(xs)/sizeof((xs)[0]))
#define LIT(x) x,sizeof(x)

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

const char* getenv_mandatory(const char* const env);

// returns current time formated as compact ISO8601: 20190123T182628Z
const char* now_iso8601(void);

void set_blocking(int fd, int blocking);
void no_new_privs(void);
