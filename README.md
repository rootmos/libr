# libr
[![Build and test](https://github.com/rootmos/libr/actions/workflows/test.yaml/badge.svg)](https://github.com/rootmos/libr/actions/workflows/test.yaml)

Opinionated public domain C snippets and helpers,
bundled as a [single-file library](https://github.com/nothings/stb).

## Usage
Select a few modules: from the list below or run `bundle -l` to list the
available modules.
For instance: `fail` and `logging`:
```shell
~/git/libr/bundle --output=r.h fail logging
```
Then `hello.c` can check for errors and do logging:
```c
#define LIBR_IMPLEMENTATION
#include "r.h"

int main(int argc, char* argv[])
{
    if(argc != 2) {
        failwith("incorrect number of arguments: %d != 2", argc);
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
20221107T142200Z:3972731:main:hello.c:10 hello libr world!
```

## Modules
- **util**: LENGTH, LIT, MAX, MIN macros<br> [.h](modules/util.h)
- **now**: current time in ISO8601 format<br> [.h](modules/now.h) [.c](modules/now.c) [wiki](https://en.wikipedia.org/wiki/ISO_8601)
- **logging**: compiletime eliminated printf-style logging<br> [.h](modules/logging.h) [.c](modules/logging.c)
- **fail**: abort-based error handling<br> [.h](modules/fail.h) [.c](modules/fail.c) [man](https://man.archlinux.org/man/abort.3.en)
- **xorshift**: implementation of xorshift64 and xorshift128+<br> [.h](modules/xorshift.h) [.c](modules/xorshift.c) [.test.c](modules/xorshift.test.c) [wiki](https://en.wikipedia.org/wiki/Xorshift)
- **crc**: implementation of CRC16-ccitt<br> [.h](modules/crc.h) [.c](modules/crc.c) [.test.c](modules/crc.test.c) [wiki](https://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks)
- **base32**: implementation of the RFC4648 Base32 encoding<br> [.h](modules/base32.h) [.c](modules/base32.c) [.test.c](modules/base32.test.c) [reference](https://datatracker.ietf.org/doc/html/rfc4648)
- **mark**: realtime frequency calcuration<br> [.h](modules/mark.h) [.c](modules/mark.c)
- **stopwatch**: pausable realtime frequency calcuration<br> [.h](modules/stopwatch.h) [.c](modules/stopwatch.c)
- **test**: test utilities<br> [.h](modules/test.h) [.c](modules/test.c)
- **file**: read file and split into lines<br> [.h](modules/file.h) [.c](modules/file.c)
- **landlock**: Linux landlock syscall wrappers and helpers<br> [.h](modules/landlock.h) [.c](modules/landlock.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/landlock.html) [man](https://man.archlinux.org/man/landlock.7.en)
- **seccomp**: seccomp syscall wrapper<br> [.h](modules/seccomp.h) [.c](modules/seccomp.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html) [man](https://man.archlinux.org/man/seccomp.2.en)
- **lua**: Lua error checking and stack helpers<br> [.h](modules/lua.h) [reference](https://www.lua.org/manual/5.4/manual.html#4.4)
- **uv**: uv error-checking<br> [.h](modules/uv.h) [reference](http://docs.libuv.org/en/v1.x/errors.html#c.uv_err_name)
- **rlimit**: apply strict rlimits<br> [.h](modules/rlimit.h) [.c](modules/rlimit.c) [man](https://man.archlinux.org/man/setrlimit.2.en)
- **env**: getenv wrappers<br> [.h](modules/env.h) [.c](modules/env.c) [man](https://man.archlinux.org/man/getenv.3.en)
- **no_new_privs**: prctl(PR_SET_NO_NEW_PRIVS) helper<br> [.h](modules/no_new_privs.h) [.c](modules/no_new_privs.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/no_new_privs.html) [man](https://man.archlinux.org/man/prctl.2.en#PR_SET_NO_NEW_PRIVS)
- **nonblock**: helper to set/unset O_NONBLOCK flag<br> [.h](modules/nonblock.h) [.c](modules/nonblock.c) [man](https://man.archlinux.org/man/fcntl.2.en#F_SETFL)
- **char**: helpers to interpret ASCII characters<br> [.h](modules/char.h)
- **devnull**: /dev/null as a filedescriptor<br> [.h](modules/devnull.h) [.c](modules/devnull.c) [man](https://man.archlinux.org/man/zero.4.en)
