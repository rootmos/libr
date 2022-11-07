#define LIBR_IMPLEMENTATION
#include "r.h"

int main(int argc, char* argv[])
{
    if(argc != 2) {
        failwith("incorrect number of arguments");
    }

    info("hello %s world!", argv[1]);

    int r = close(0);
    CHECK(r, "close(%d)", 0);

    void* buf = malloc(4096); CHECK_MALLOC(buf);

    return 0;
}
