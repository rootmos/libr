# libr
[![Build and test](https://github.com/rootmos/libr/actions/workflows/test.yaml/badge.svg)](https://github.com/rootmos/libr/actions/workflows/test.yaml)

Opinionated public domain C snippets and helpers,
bundled as a [single-file library](https://github.com/nothings/stb).

## Usage
Select a few modules, for instance `fail` and `logging`:
```shell
~/git/libr/bundle --output=r.h fail logging
```
Then for example `hello.c`:
```c
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
```
which when built and run:
```shell
gcc -DLOG_LEVEL=LOG_INFO -o hello hello.c
./hello libr
```
says hello:
```
20221107T093336Z:3138442:main:hello.c:10 hello libr world!
```

## Modules
- **util**: LENGTH, LIT, MAX, MIN macros<br> [.h](modules/util.h)
- **now**: current time in ISO8601 format<br> [.h](modules/now.h) [.c](modules/now.c) [wiki](https://en.wikipedia.org/wiki/ISO_8601)
- **logging**<br> [.h](modules/logging.h) [.c](modules/logging.c)
- **fail**: abort-based error handling<br> [.h](modules/fail.h) [.c](modules/fail.c) [man](https://man.archlinux.org/man/abort.3.en)
- **xorshift**: implementation of xorshift64 and xorshift128+<br> [.h](modules/xorshift.h) [.c](modules/xorshift.c) [.test.c](modules/xorshift.test.c) [wiki](https://en.wikipedia.org/wiki/Xorshift)
- **crc**: implementation of CRC16-ccitt<br> [.h](modules/crc.h) [.c](modules/crc.c) [.test.c](modules/crc.test.c) [wiki](https://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks)
- **base32**: implementation of the RFC4648 Base32 encoding<br> [.h](modules/base32.h) [.c](modules/base32.c) [.test.c](modules/base32.test.c) [reference](https://datatracker.ietf.org/doc/html/rfc4648)
- **stopwatch**<br> [.h](modules/stopwatch.h) [.c](modules/stopwatch.c)
- **mark**<br> [.h](modules/mark.h) [.c](modules/mark.c)
- **test**<br> [.h](modules/test.h) [.c](modules/test.c)
- **file**<br> [.h](modules/file.h) [.c](modules/file.c)
- **landlock**: Linux landlock syscall wrappers and helpers<br> [.h](modules/landlock.h) [.c](modules/landlock.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/landlock.html) [man](https://man.archlinux.org/man/landlock.7.en)
- **seccomp**: seccomp syscall wrapper<br> [.h](modules/seccomp.h) [.c](modules/seccomp.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html) [man](https://man.archlinux.org/man/seccomp.2.en)
- **lua**<br> [.h](modules/lua.h)
- **uv**<br> [.h](modules/uv.h)
- **rlimit**: apply strict rlimits<br> [.h](modules/rlimit.h) [.c](modules/rlimit.c) [man](https://man.archlinux.org/man/setrlimit.2.en)
- **env**: getenv wrappers<br> [.h](modules/env.h) [.c](modules/env.c) [man](https://man.archlinux.org/man/getenv.3.en)
- **no_new_privs**: prctl(PR_SET_NO_NEW_PRIVS) helper<br> [.h](modules/no_new_privs.h) [.c](modules/no_new_privs.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/no_new_privs.html) [man](https://man.archlinux.org/man/prctl.2.en#PR_SET_NO_NEW_PRIVS)
- **nonblock**: helper to set/unset O_NONBLOCK flag<br> [.h](modules/nonblock.h) [.c](modules/nonblock.c) [man](https://man.archlinux.org/man/fcntl.2.en#F_SETFL)
- **char**: helpers to interpret ASCII characters<br> [.h](modules/char.h)
