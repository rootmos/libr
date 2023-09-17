TEST_SUITE(sha256, {
    TEST(empty, {
        struct sha256_state st;
        sha256_init(&st);
        sha256_finalize(&st);

        uint8_t expected[SHA256_DIGEST_LENGTH];
        assert(hex_decode(expected, "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") == SHA256_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(foo, {
        struct sha256_state st;
        sha256_init(&st);
        sha256_update(&st, STR("foo"));
        sha256_finalize(&st);

        uint8_t expected[SHA256_DIGEST_LENGTH];
        assert(hex_decode(expected, "2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae") == SHA256_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(foobar, {
        struct sha256_state st;
        sha256_init(&st);
        sha256_update(&st, STR("foo"));
        sha256_update(&st, STR("bar"));
        sha256_finalize(&st);

        uint8_t expected[SHA256_DIGEST_LENGTH];
        assert(hex_decode(expected, "c3ab8ff13720e8ad9047dd39466b3c8974e592c2fa383d4a3960714caef0c4f2") == SHA256_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });
})
