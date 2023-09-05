#include <assert.h>

TEST_SUITE(base32, {
    TEST(encode_empty, {
         char enc[1];
         assert(BASE32_ENCODED_LENGTH(0) == sizeof(enc));
         assert(base32_encode(enc, NULL, 0) == 1);
         assert(strcmp(enc, "") == 0);
    });

    TEST(encode_f, {
         char enc[8+1];
         assert(BASE32_ENCODED_LENGTH(1) == sizeof(enc));
         assert(base32_encode(enc, "f", 1) == sizeof(enc));
         assert(strcmp(enc, "MY======") == 0);
    });

    TEST(encode_fo, {
         char enc[8+1];
         assert(BASE32_ENCODED_LENGTH(2) == sizeof(enc));
         assert(base32_encode(enc, "fo", 2) == sizeof(enc));
         assert(strcmp(enc, "MZXQ====") == 0);
    });

    TEST(encode_foo, {
         char enc[8+1];
         assert(BASE32_ENCODED_LENGTH(3) == sizeof(enc));
         assert(base32_encode(enc, "foo", 3) == sizeof(enc));
         assert(strcmp(enc, "MZXW6===") == 0);
    });

    TEST(encode_foob, {
         char enc[8+1];
         assert(BASE32_ENCODED_LENGTH(4) == sizeof(enc));
         assert(base32_encode(enc, "foob", 4) == sizeof(enc));
         assert(strcmp(enc, "MZXW6YQ=") == 0);
    });

    TEST(encode_fooba, {
         char enc[8+1];
         assert(BASE32_ENCODED_LENGTH(5) == sizeof(enc));
         assert(base32_encode(enc, "fooba", 5) == sizeof(enc));
         assert(strcmp(enc, "MZXW6YTB") == 0);
    });

    TEST(encode_foobar, {
         char enc[16+1];
         assert(BASE32_ENCODED_LENGTH(6) == sizeof(enc));
         assert(base32_encode(enc, "foobar", 6) == sizeof(enc));
         assert(strcmp(enc, "MZXW6YTBOI======") == 0);
    });

    TEST(encode_sentence, {
         char enc[32+1];
         assert(BASE32_ENCODED_LENGTH(17) == sizeof(enc));
         assert(base32_encode(enc, "foobarbazwoopwoop", 17) == sizeof(enc));
         assert(strcmp(enc, "MZXW6YTBOJRGC6TXN5XXA53PN5YA====") == 0);
    });

    TEST(decode_empty, {
         char* enc = "";
         unsigned char buf[0];
         assert(base32_decode(buf, enc, strlen(enc)) == 0);
    });

    TEST(decode_f, {
         const char* enc = "MY=====";
         char buf[1];
         assert(base32_decode(buf, enc, strlen(enc)) == sizeof(buf));
         assert(strncmp(buf, "f", sizeof(buf)) == 0);
    });

    TEST(decode_fo, {
         const char* enc = "MZXQ====";
         char buf[2];
         assert(base32_decode(buf, enc, strlen(enc)) == sizeof(buf));
         assert(strncmp(buf, "fo", sizeof(buf)) == 0);
    });

    TEST(decode_foo, {
         const char* enc = "MZXW6===";
         char buf[3];
         assert(base32_decode(buf, enc, strlen(enc)) == sizeof(buf));
         assert(strncmp(buf, "foo", sizeof(buf)) == 0);
    });

    TEST(decode_foob, {
         const char* enc = "MZXW6YQ=";
         char buf[4];
         assert(base32_decode(buf, enc, strlen(enc)) == sizeof(buf));
         assert(strncmp(buf, "foob", sizeof(buf)) == 0);
    });

    TEST(decode_fooba, {
         const char* enc = "MZXW6YTB";
         char buf[5];
         assert(base32_decode(buf, enc, strlen(enc)) == sizeof(buf));
         assert(strncmp(buf, "fooba", sizeof(buf)) == 0);
    });

    TEST(decode_foobar, {
         const char* enc = "MZXW6YTBOI======";
         char buf[6];
         assert(base32_decode(buf, enc, strlen(enc)) == sizeof(buf));
         assert(strncmp(buf, "foobar", sizeof(buf)) == 0);
    });

    TEST(decode_sentence, {
         const char* enc = "MZXW6YTBOJRGC6TXN5XXA53PN5YA====";
         char buf[17];
         assert(base32_decode(buf, enc, strlen(enc)) == sizeof(buf));
         assert(strncmp(buf, "foobarbazwoopwoop", sizeof(buf)) == 0);
    });

    TEST(round_trip_arbitrary_byte_string, {
         for(size_t i = 0; i < 100; i++) {
             uint16_t L; random_bytes(&L, sizeof(L));
             char msg[L]; random_bytes(msg, sizeof(msg));

             char enc[BASE32_ENCODED_LENGTH(sizeof(msg))];
             assert(base32_encode(enc, msg, L) == sizeof(enc));

             char dec[L];
             assert(base32_decode(dec, enc, sizeof(enc)) == L);

             assert(memcmp(msg, dec, L) == 0);
         }
    });
})
