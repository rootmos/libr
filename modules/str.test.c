TEST_SUITE(str, {
    TEST(endswith_empty, {
         assert(endswith("", "suffix") == -1);
    });

    TEST(endswith_whole, {
         assert(endswith("foo", "foo") == 0);
    });

    TEST(endswith_true, {
         assert(endswith("foo.bar", ".bar") == 3);
    });

    TEST(endswith_false, {
         assert(endswith("foo.baz", ".bar") == -1);
    });


    TEST(parse_bytes_1, {
         assert(parse_bytes("1", NULL) == 1);
    });

    TEST(parse_bytes_1k, {
         assert(parse_bytes("1k", NULL) == 1000);
    });

    TEST(parse_bytes_1K, {
         assert(parse_bytes("1K", NULL) == 1024);
    });

    TEST(parse_bytes_spaces, {
         assert(parse_bytes(" 1   M  ", NULL) == 1024*1024);
    });

    TEST(parse_bytes_empty, {
         assert(parse_bytes("", NULL) < 0);
    });

    TEST(parse_bytes_invalid_suffix, {
         assert(parse_bytes("1q", NULL) < 0);
    });
})
