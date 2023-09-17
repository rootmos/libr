#include "hexdump.h"

#include <stdint.h>
#include <stdio.h>

void hexdump(int fd, void* buf, size_t l)
{
    uint8_t* p = buf;
    for(int i = 0; i < l; i++) {
        if(i % 16 == 0) {
            if(i > 0) {
                dprintf(fd, "\n");
            }
            dprintf(fd, "%02x: ", i);
        }
        dprintf(fd, "%02x", p[i]);
        if(i % 16 != 0 && (i+1) % 4 == 0) {
            dprintf(fd, " ");
        }
    }
    dprintf(fd, "\n");
}
