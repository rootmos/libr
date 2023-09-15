#include "hex.h"

#include <stdint.h>

static const char* const hex_alphabeth_upper_case = "0123456789ABCDEF";
static const char* const hex_alphabeth_lower_case = "0123456789abcdef";
const char* hex_alphabeth = hex_alphabeth_lower_case;

void hex_encode(char* enc, const void* bin, size_t l)
{
    for(size_t i = 0; i < l; i++) {
        uint8_t b = ((uint8_t*)bin)[i];
        *(enc++) = hex_alphabeth[b >> 4];
        *(enc++) = hex_alphabeth[b & 0b1111];
    }
    *enc = 0;
}

static int_fast8_t from_hex_digit(char c)
{
    if('0' <= c && c <= '9') {
        return c - '0';
    }
    if('a' <= c && c <= 'f') {
        return 10 + c - 'a';
    }
    if('A' <= c && c <= 'F') {
        return 10 + c - 'A';
    }
    return -1;
}

ssize_t hex_decode(void* bin, const char* enc)
{
    for(size_t i = 0, j = 0;;) {
        if(enc[i] == 0) {
            return j;
        }
        int_fast8_t u = from_hex_digit(enc[i++]);
        if(u < 0) {
            return -i;
        }

        int_fast8_t l = from_hex_digit(enc[i++]);
        if(l < 0) {
            return -i;
        }

        if(bin != NULL) {
            ((uint8_t*)bin)[j++] = u << 4 | l;
        }
    }
}
