#pragma once

#include <stddef.h>
#include <sys/types.h>

#define HEX_ENCODED_LENGTH(BIN_LEN) ((BIN_LEN)*2+1)
#define HEX_DECODED_LENGTH(HEX_LEN) ((BIN_LEN)*2)

void LIBR(hex_encode)(char* enc, const void* bin, size_t l);
ssize_t LIBR(hex_decode)(void* bin, const char* enc);
