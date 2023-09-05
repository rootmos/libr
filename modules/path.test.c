#include <assert.h>

int main(int argc, char* argv[])
{
    TEST(path_one_segment, {
        char buf[128];
        size_t r = path_join(LIT(buf), "foo", NULL);
        assert(r == 3);
        assert(strcmp(buf, "foo") == 0);
    });

    TEST(path_two_segments, {
        char buf[128];
        size_t r = path_join(LIT(buf), "foo", "bar", NULL);
        assert(r == 7);
        assert(strcmp(buf, "foo/bar") == 0);
    });

    TEST(path_three_segments, {
        char buf[128];
        size_t r = path_join(LIT(buf), "foo", "bar", "baz", NULL);
        assert(r == 11);
        assert(strcmp(buf, "foo/bar/baz") == 0);
    });

    TEST(path_overflow_first_segment, {
        char buf[3] = {0};
        size_t r = path_join(LIT(buf), "foobar", NULL);
        assert(r == 6);
        assert(buf[0] == 0);
    });

    TEST(path_overflow_divider, {
        char buf[4] = {0};
        size_t r = path_join(LIT(buf), "foo", "bar", NULL);
        assert(r == 7);
        assert(strncmp(buf, "foo", 3) == 0);
    });

    TEST(path_overflow_subsequent_segments, {
        char buf[6] = {0};
        size_t r = path_join(LIT(buf), "foo", "bar", NULL);
        assert(r == 7);
        assert(strcmp(buf, "foo/") == 0);
    });
}
