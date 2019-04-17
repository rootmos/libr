void xorshift_tests(void);
void crc_tests(void);
void base32_tests(void);

int main(int argc, char** argv)
{
    xorshift_tests();
    crc_tests();
    base32_tests();
    return 0;
}
