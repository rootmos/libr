TEST_SUITE(sha1, {
    TEST(empty, {
        struct sha1_state st;
        sha1_init(&st);
        sha1_finalize(&st);

        uint8_t expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "da39a3ee5e6b4b0d3255bfef95601890afd80709") == SHA1_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(foo, {
        struct sha1_state st;
        sha1_init(&st);
        sha1_update(&st, "foo", 3);
        sha1_finalize(&st);

        char expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "0beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33") == SHA1_DIGEST_LENGTH);

        char actual[SHA1_DIGEST_LENGTH*2+1];
        hex_encode(actual, LIT(st.digest));
        dprintf(2, "actual=%s\n", actual);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(foobar, {
        struct sha1_state st;
        sha1_init(&st);
        sha1_update(&st, "foo", 3);
        sha1_update(&st, "bar", 3);
        sha1_finalize(&st);

        char expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "8843d7f92416211de9ebb963ff4ce28125932878") == SHA1_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });
})
