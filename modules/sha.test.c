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
        sha1_update(&st, STR("foo"));
        sha1_finalize(&st);

        char expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "0beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33") == SHA1_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(foobar, {
        struct sha1_state st;
        sha1_init(&st);
        sha1_update(&st, STR("foo"));
        sha1_update(&st, STR("bar"));
        sha1_finalize(&st);

        char expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "8843d7f92416211de9ebb963ff4ce28125932878") == SHA1_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(rfc3174_test1, {
        struct sha1_state st;
        sha1_init(&st);
        sha1_update(&st, STR("abc"));
        sha1_finalize(&st);

        char expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "A9993E364706816ABA3E25717850C26C9CD0D89D") == SHA1_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(rfc3174_test2, {
        struct sha1_state st;
        sha1_init(&st);
        sha1_update(&st, STR("abcdbcdecdefdefgefghfghighijhi"));
        sha1_update(&st, STR("jkijkljklmklmnlmnomnopnopq"));
        sha1_finalize(&st);

        char expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "84983E441C3BD26EBAAE4AA1F95129E5E54670F1") == SHA1_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(rfc3174_test3, {
        struct sha1_state st;
        sha1_init(&st);
        for(size_t i = 0; i < 1000000; i++) {
            sha1_update(&st, STR("a"));
        }
        sha1_finalize(&st);

        char expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "34AA973CD4C4DAA4F61EEB2BDBAD27316534016F") == SHA1_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });

    TEST(rfc3174_test4, {
        struct sha1_state st;
        sha1_init(&st);
        for(size_t i = 0; i < 10; i++) {
            sha1_update(&st, STR("01234567012345670123456701234567"));
            sha1_update(&st, STR("01234567012345670123456701234567"));
        }
        sha1_finalize(&st);

        char expected[SHA1_DIGEST_LENGTH];
        assert(hex_decode(expected, "DEA356A2CDDD90C7A7ECEDC5EBB563934F460452") == SHA1_DIGEST_LENGTH);

        assert(memcmp(st.digest, expected, sizeof(expected)) == 0);
    });
})
