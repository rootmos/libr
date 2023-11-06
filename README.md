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
20230906T105013Z:1530871:main:hello.c:10 hello libr world!
```

## Modules
- **base32**: implementation of the RFC4648 Base32 encoding<br> [.h](modules/base32.h) [.c](modules/base32.c) [.test.c](modules/base32.test.c) [reference](https://datatracker.ietf.org/doc/html/rfc4648)
- **char**: helpers to interpret ASCII characters<br> [.h](modules/char.h)
- **crc**: implementation of CRC16-ccitt<br> [.h](modules/crc.h) [.c](modules/crc.c) [.test.c](modules/crc.test.c) [wiki](https://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks)
- **devnull**: /dev/null as a filedescriptor<br> [.h](modules/devnull.h) [.c](modules/devnull.c) [man](https://man.archlinux.org/man/zero.4.en)
- **env**: getenv wrappers<br> [.h](modules/env.h) [.c](modules/env.c) [man](https://man.archlinux.org/man/getenv.3.en)
- **fail**: abort-based error handling<br> [.h](modules/fail.h) [.c](modules/fail.c) [man](https://man.archlinux.org/man/abort.3.en)
- **file**: read file line-by-line or as a whole<br> [.h](modules/file.h) [.c](modules/file.c)
- **hex**: hexadecimal binary encoding<br> [.h](modules/hex.h) [.c](modules/hex.c) [.test.c](modules/hex.test.c)
- **hexdump**: hexdump troubleshooting utility<br> [.h](modules/hexdump.h) [.c](modules/hexdump.c)
- **landlock**: Linux landlock syscall wrappers and helpers<br> [.h](modules/landlock.h) [.c](modules/landlock.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/landlock.html) [man](https://man.archlinux.org/man/landlock.7.en)
- **logging**: compiletime eliminated printf-style logging<br> [.h](modules/logging.h) [.c](modules/logging.c)
- **lua**: Lua error checking and stack helpers<br> [.h](modules/lua.h) [.c](modules/lua.c) [reference](https://www.lua.org/manual/5.4/manual.html#4.4)
- **mark**: realtime frequency calculation<br> [.h](modules/mark.h) [.c](modules/mark.c)
- **no_new_privs**: prctl(PR_SET_NO_NEW_PRIVS) helper<br> [.h](modules/no_new_privs.h) [.c](modules/no_new_privs.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/no_new_privs.html) [man](https://man.archlinux.org/man/prctl.2.en#PR_SET_NO_NEW_PRIVS)
- **nonblock**: helper to set/unset O_NONBLOCK flag<br> [.h](modules/nonblock.h) [.c](modules/nonblock.c) [man](https://man.archlinux.org/man/fcntl.2.en#F_SETFL)
- **now**: current time in ISO8601 format<br> [.h](modules/now.h) [.c](modules/now.c) [wiki](https://en.wikipedia.org/wiki/ISO_8601)
- **path**: path mangling<br> [.h](modules/path.h) [.c](modules/path.c) [.test.c](modules/path.test.c)
- **rlimit**: apply strict rlimits<br> [.h](modules/rlimit.h) [.c](modules/rlimit.c) [man](https://man.archlinux.org/man/setrlimit.2.en)
- **seccomp**: seccomp syscall wrapper<br> [.h](modules/seccomp.h) [.c](modules/seccomp.c) [reference](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html) [man](https://man.archlinux.org/man/seccomp.2.en)
- **sha1**: SHA-1 implementation<br> [.h](modules/sha1.h) [.c](modules/sha1.c) [.test.c](modules/sha1.test.c) [wiki](https://en.wikipedia.org/wiki/Secure_Hash_Algorithms) [reference](https://www.rfc-editor.org/rfc/rfc3174)
- **sha2**: SHA-2 implementation<br> [.h](modules/sha2.h) [.c](modules/sha2.c) [.test.c](modules/sha2.test.c) [wiki](https://en.wikipedia.org/wiki/Secure_Hash_Algorithms) [reference](https://www.rfc-editor.org/rfc/rfc6234)
- **stopwatch**: pausable realtime frequency calculation<br> [.h](modules/stopwatch.h) [.c](modules/stopwatch.c)
- **str**: string helpers<br> [.h](modules/str.h) [.c](modules/str.c) [.test.c](modules/str.test.c)
- **test**: test utilities<br> [.h](modules/test.h) [.c](modules/test.c)
- **util**: LENGTH, LIT, MAX, MIN macros<br> [.h](modules/util.h)
- **uv**: uv error-checking<br> [.h](modules/uv.h) [reference](http://docs.libuv.org/en/v1.x/errors.html#c.uv_err_name)
- **xdg**: XDG base directories<br> [.h](modules/xdg.h) [.c](modules/xdg.c) [reference](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html)
- **xorshift**: implementation of xorshift64 and xorshift128+<br> [.h](modules/xorshift.h) [.c](modules/xorshift.c) [.test.c](modules/xorshift.test.c) [wiki](https://en.wikipedia.org/wiki/Xorshift)
