#include <r.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void base32_tests(void)
{
    TEST(base32_encode_empty, {
         char enc[1];
         assert(base32_encode(enc, NULL, 0) == 1);
         assert(strcmp(enc, "") == 0);
    });

    TEST(base32_encode_f, {
         char enc[8+1];
         assert(base32_encode(enc, "f", 1) == sizeof(enc));
         assert(strcmp(enc, "MY======") == 0);
    });

    TEST(base32_encode_fo, {
         char enc[8+1];
         assert(base32_encode(enc, "fo", 2) == sizeof(enc));
         assert(strcmp(enc, "MZXQ====") == 0);
    });

    TEST(base32_encode_foo, {
         char enc[8+1];
         assert(base32_encode(enc, "foo", 3) == sizeof(enc));
         assert(strcmp(enc, "MZXW6===") == 0);
    });

    TEST(base32_encode_foob, {
         char enc[8+1];
         assert(base32_encode(enc, "foob", 4) == sizeof(enc));
         assert(strcmp(enc, "MZXW6YQ=") == 0);
    });

    TEST(base32_encode_fooba, {
         char enc[8+1];
         assert(base32_encode(enc, "fooba", 5) == sizeof(enc));
         assert(strcmp(enc, "MZXW6YTB") == 0);
    });

    TEST(base32_encode_foobar, {
         char enc[16+1];
         assert(base32_encode(enc, "foobar", 6) == sizeof(enc));
         assert(strcmp(enc, "MZXW6YTBOI======") == 0);
    });
}
