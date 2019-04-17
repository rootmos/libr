#pragma once

// https://tools.ietf.org/html/rfc4648

size_t base32_encode(char* out, const void* data, size_t l);
