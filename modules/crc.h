#pragma once

#include <stdint.h>
#include <stddef.h>

uint16_t LIBR(crc16_ccitt)(const void* msg, size_t l);
