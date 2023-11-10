#include "str.h"

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t endswith(const char* str, const char* suffix)
{
    size_t l = strlen(str), k = strlen(suffix);
    if(k > l || strcmp(str + l - k, suffix) != 0) {
        return -1;
    }
    return l - k;
}

long long parse_bytes(const char* str, const char** err)
{
    static char errbuf[128];

    long long sum = -1;
    const char* s = str;

    for(;;s++) {
        for(char c = *s; isspace(c); c = *++s);

        char* e;
        long long l = strtoll(s, &e, 10);
        if(s == e) {
            if(sum < 0) {
                if(err) {
                    int r = snprintf(LIT(errbuf), "no digits (:%lu)", s - str);
                    if(r >= sizeof(errbuf)) {
                        return -2;
                    }
                    *err = errbuf;
                }
            }
            return sum;
        }

        if(sum < 0) sum = 0;
        s = e;

        for(char c = *s; isspace(c); c = *++s);

        switch(*s) {
        case 0: sum += l; return sum;
        case 'k': sum += l * 1000; break;
        case 'K': sum += l * 1024; break;
        case 'm': sum += l * 1000*1000; break;
        case 'M': sum += l * 1024*1024; break;
        case 'g': sum += l * 1000*1000*1000; break;
        case 'G': sum += l * 1024*1024*1024; break;
        case 't': sum += l * 1000*1000*1000*1000; break;
        case 'T': sum += l * 1024*1024*1024*1024; break;
        default:
            if(err) {
                int r = snprintf(LIT(errbuf), "not a valid suffix %c (:%lu)", *s, s - str);
                if(r >= sizeof(errbuf)) {
                    return -2;
                }
                *err = errbuf;
            }
            return -1;
        }
    }
}

ssize_t render_bytes(char* buf, size_t len, size_t bs)
{
    if(bs == 0) {
        if(len < 2) {
            return -1;
        }
        return snprintf(buf, len, "0");
    }

    struct {
        char* suffix;
        size_t bytes;
    } suffixes[] = {
        { "T", (size_t)1024*1024*1024*1024 },
        { "G", (size_t)1024*1024*1024 },
        { "M", (size_t)1024*1024 },
        { "K", (size_t)1024 },
        { "", (size_t)1 },
    };

    size_t l = len;
    char* b = buf;
    for(size_t i = 0; i < LENGTH(suffixes); i++) {
        if(bs >= suffixes[i].bytes) {
            size_t q = bs / suffixes[i].bytes;
            bs -= q * suffixes[i].bytes;
            int r = snprintf(b, len, "%zu%s", q, suffixes[i].suffix);
            if(r >= l) {
                return -1;
            }
            b += r;
            l -= r;
        }

    }

    return len - l;
}
