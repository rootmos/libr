#pragma once

#include <stddef.h>

#define HEX_ENCODED_LENGTH(BIN_LEN) ((BIN_LEN)*2+1)
#define HEX_DECODED_LENGTH(HEX_LEN) ((BIN_LEN)*2)

void hex_encode(char* enc, const void* bin, size_t l);
size_t hex_decode(void* bin, const char* enc);

#ifdef failwith
char* hex_encode_s(const void* bin, size_t l);
char* hex_encode_m(const void* bin, size_t l);
void* hex_decode_s(void* bin, size_t* l);
void* hex_decode_m(void* bin, size_t* l);
#endif
