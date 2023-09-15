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

size_t hex_decode(void* bin, const char* enc)
{
    return -1;
}
