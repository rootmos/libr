#include <assert.h>

TEST_SUITE(crc, {
    TEST(crc16_ccitt_empty, {
         assert(LIBR(crc16_ccitt)(NULL, 0) == 0);
    });

    TEST(crc16_ccitt_one, {
         unsigned char msg = 1;
         assert(LIBR(crc16_ccitt)(&msg, 1) == 0x1021);
    });

    TEST(crc16_ccitt_123456789, {
         assert(LIBR(crc16_ccitt)("123456789", 9) == 0x31c3);
    });

})
