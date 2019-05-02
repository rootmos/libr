#pragma once

#include <unistd.h>

// https://tools.ietf.org/html/rfc4648

#define BASE32_ENCODED_LENGTH(BIN_LEN) ((((BIN_LEN)/5)+((BIN_LEN)%5!=0))*8+1)

size_t base32_encode(char* out, const void* data, size_t l);
ssize_t base32_decode(void* out, const char* enc, size_t enc_len);
