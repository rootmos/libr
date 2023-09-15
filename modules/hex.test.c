#include <assert.h>

TEST_SUITE(hex, {
    hex_alphabeth = hex_alphabeth_lower_case;

    TEST(encode_lower_empty, {
        char enc[1] = {0};
        assert(HEX_ENCODED_LENGTH(0) == sizeof(enc));
        hex_encode(enc, NULL, 0);
        assert(strcmp(enc, "") == 0);
    });

    TEST(encode_lower_f, {
        char enc[2+1] = {0};
        assert(HEX_ENCODED_LENGTH(1) == sizeof(enc));
        hex_encode(enc, "f", 1);
        assert(strcmp(enc, "66") == 0);
    });

    TEST(encode_lower_fo, {
        char enc[4+1] = {0};
        assert(HEX_ENCODED_LENGTH(2) == sizeof(enc));
        hex_encode(enc, "fo", 2);
        assert(strcmp(enc, "666f") == 0);
    });

    TEST(encode_lower_foo, {
        char enc[6+1] = {0};
        assert(HEX_ENCODED_LENGTH(3) == sizeof(enc));
        hex_encode(enc, "foo", 3);
        assert(strcmp(enc, "666f6f") == 0);
    });

    TEST(encode_lower_foob, {
        char enc[8+1] = {0};
        assert(HEX_ENCODED_LENGTH(4) == sizeof(enc));
        hex_encode(enc, "foob", 4);
        assert(strcmp(enc, "666f6f62") == 0);
    });

    TEST(encode_lower_fooba, {
        char enc[10+1] = {0};
        assert(HEX_ENCODED_LENGTH(5) == sizeof(enc));
        hex_encode(enc, "fooba", 5);
        assert(strcmp(enc, "666f6f6261") == 0);
    });

    TEST(encode_lower_foobar, {
        char enc[12+1] = {0};
        assert(HEX_ENCODED_LENGTH(6) == sizeof(enc));
        hex_encode(enc, "foobar", 6);
        assert(strcmp(enc, "666f6f626172") == 0);
    });


    hex_alphabeth = hex_alphabeth_upper_case;

    TEST(encode_upper_empty, {
        char enc[1] = {0};
        assert(HEX_ENCODED_LENGTH(0) == sizeof(enc));
        hex_encode(enc, NULL, 0);
        assert(strcmp(enc, "") == 0);
    });

    TEST(encode_upper_f, {
        char enc[2+1] = {0};
        assert(HEX_ENCODED_LENGTH(1) == sizeof(enc));
        hex_encode(enc, "f", 1);
        assert(strcmp(enc, "66") == 0);
    });

    TEST(encode_upper_fo, {
        char enc[4+1] = {0};
        assert(HEX_ENCODED_LENGTH(2) == sizeof(enc));
        hex_encode(enc, "fo", 2);
        assert(strcmp(enc, "666F") == 0);
    });

    TEST(encode_upper_foo, {
        char enc[6+1] = {0};
        assert(HEX_ENCODED_LENGTH(3) == sizeof(enc));
        hex_encode(enc, "foo", 3);
        assert(strcmp(enc, "666F6F") == 0);
    });

    TEST(encode_upper_foob, {
        char enc[8+1] = {0};
        assert(HEX_ENCODED_LENGTH(4) == sizeof(enc));
        hex_encode(enc, "foob", 4);
        assert(strcmp(enc, "666F6F62") == 0);
    });

    TEST(encode_upper_fooba, {
        char enc[10+1] = {0};
        assert(HEX_ENCODED_LENGTH(5) == sizeof(enc));
        hex_encode(enc, "fooba", 5);
        assert(strcmp(enc, "666F6F6261") == 0);
    });

    TEST(encode_upper_foobar, {
        char enc[12+1] = {0};
        assert(HEX_ENCODED_LENGTH(6) == sizeof(enc));
        hex_encode(enc, "foobar", 6);
        assert(strcmp(enc, "666F6F626172") == 0);
    });

    TEST(decode_empty, {
        assert(hex_decode(NULL, "") == 0);
    });

    TEST(decode_lower_f, {
        char bin[1] = {0};
        assert(hex_decode(bin, "66") == 1);
        assert(memcmp(bin, "f", 1) == 0);
    });

    TEST(decode_lower_fo, {
        char bin[2] = {0};
        assert(hex_decode(bin, "666f") == 2);
        assert(memcmp(bin, "fo", 2) == 0);
    });

    TEST(decode_lower_foo, {
        char bin[3] = {0};
        assert(hex_decode(bin, "666f6f") == 3);
        assert(memcmp(bin, "foo", 3) == 0);
    });

    TEST(decode_lower_foob, {
        char bin[4] = {0};
        assert(hex_decode(bin, "666f6f62") == 4);
        assert(memcmp(bin, "fooba", 4) == 0);
    });

    TEST(decode_lower_fooba, {
        char bin[5] = {0};
        assert(hex_decode(bin, "666f6f6261") == 5);
        assert(memcmp(bin, "fooba", 5) == 0);
    });

    TEST(decode_lower_foobar, {
        char bin[6] = {0};
        assert(hex_decode(bin, "666f6f626172") == 6);
        assert(memcmp(bin, "foobar", 6) == 0);
    });

    TEST(decode_upper_f, {
        char bin[1] = {0};
        assert(hex_decode(bin, "66") == 1);
        assert(memcmp(bin, "f", 1) == 0);
    });

    TEST(decode_upper_fo, {
        char bin[2] = {0};
        assert(hex_decode(bin, "666F") == 2);
        assert(memcmp(bin, "fo", 2) == 0);
    });

    TEST(decode_upper_foo, {
        char bin[3] = {0};
        assert(hex_decode(bin, "666F6F") == 3);
        assert(memcmp(bin, "foo", 3) == 0);
    });

    TEST(decode_upper_foob, {
        char bin[4] = {0};
        assert(hex_decode(bin, "666F6F62") == 4);
        assert(memcmp(bin, "fooba", 4) == 0);
    });

    TEST(decode_upper_fooba, {
        char bin[5] = {0};
        assert(hex_decode(bin, "666f6F6261") == 5);
        assert(memcmp(bin, "fooba", 5) == 0);
    });

    TEST(decode_upper_foobar, {
        char bin[6] = {0};
        assert(hex_decode(bin, "666F6F626172") == 6);
        assert(memcmp(bin, "foobar", 6) == 0);
    });

    TEST(decode_malformed, {
        assert(hex_decode(NULL, "r") == -1);
        assert(hex_decode(NULL, "0") == -2);
        assert(hex_decode(NULL, "0102r") == -5);
        assert(hex_decode(NULL, "0102f") == -6);
    });

    TEST(decode_returns_length, {
        const char* s = "666F6F626172";
        char bin[hex_decode(NULL, s)];
        memset(bin, 0, sizeof(bin));
        assert(memcmp(bin, "foobar", sizeof(bin)) == 0);
    });
})
