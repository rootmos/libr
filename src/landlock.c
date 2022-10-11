#include "r/fail.h"
#include "r/util.h"

#include <fcntl.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <linux/landlock.h>

#ifndef landlock_create_ruleset
static inline int landlock_create_ruleset(
    const struct landlock_ruleset_attr* const attr,
    const size_t size, const __u32 flags)
{
    return syscall(__NR_landlock_create_ruleset, attr, size, flags);
}
#endif

#ifndef landlock_add_rule
static inline int landlock_add_rule(const int ruleset_fd,
                                    const enum landlock_rule_type rule_type,
                                    const void* const rule_attr,
                                    const __u32 flags)
{
    return syscall(__NR_landlock_add_rule, ruleset_fd, rule_type, rule_attr,
                   flags);
}
#endif

#ifndef landlock_restrict_self
static inline int landlock_restrict_self(const int ruleset_fd,
                                         const __u32 flags)
{
    return syscall(__NR_landlock_restrict_self, ruleset_fd, flags);
}
#endif

int landlock_abi_version(void)
{
    int r = landlock_create_ruleset(
        NULL, 0, LANDLOCK_CREATE_RULESET_VERSION);
    CHECK(r, "landlock ABI version");
    return r;
    if(r < 1) {
        failwith("landlock not supported (ABI=%d)", r);
    }
}

int landlock_new_ruleset(void)
{
    struct landlock_ruleset_attr rs = {
        .handled_access_fs =
              LANDLOCK_ACCESS_FS_EXECUTE
            | LANDLOCK_ACCESS_FS_WRITE_FILE
            | LANDLOCK_ACCESS_FS_READ_FILE
            | LANDLOCK_ACCESS_FS_READ_DIR
            | LANDLOCK_ACCESS_FS_REMOVE_DIR
            | LANDLOCK_ACCESS_FS_REMOVE_FILE
            | LANDLOCK_ACCESS_FS_MAKE_CHAR
            | LANDLOCK_ACCESS_FS_MAKE_DIR
            | LANDLOCK_ACCESS_FS_MAKE_REG
            | LANDLOCK_ACCESS_FS_MAKE_SOCK
            | LANDLOCK_ACCESS_FS_MAKE_FIFO
            | LANDLOCK_ACCESS_FS_MAKE_BLOCK
            | LANDLOCK_ACCESS_FS_MAKE_SYM
            // TODO: | LANDLOCK_ACCESS_FS_REFER,
    };

    int rsfd = landlock_create_ruleset(&rs, sizeof(rs), 0);
    CHECK(rsfd, "landlock_create_ruleset");

    return rsfd;
}

void landlock_allow(int rsfd, const char* path, __u64 allowed_access)
{
    struct landlock_path_beneath_attr pb = {
        .allowed_access = allowed_access,
    };

    pb.parent_fd = open(path, __O_PATH|O_CLOEXEC);
    CHECK(pb.parent_fd, "open(%s, O_PATH)", path);

    int r = landlock_add_rule(rsfd, LANDLOCK_RULE_PATH_BENEATH, &pb, 0);
    CHECK(r, "landlock_add_rule");

    r = close(pb.parent_fd); CHECK(r, "close(%s)", path);
}

void landlock_allow_read(int rsfd, const char* path)
{
    landlock_allow(rsfd, path,
        LANDLOCK_ACCESS_FS_READ_FILE
    );
}

void landlock_allow_read_write(int rsfd, const char* path)
{
    landlock_allow(rsfd, path,
        LANDLOCK_ACCESS_FS_READ_FILE
      | LANDLOCK_ACCESS_FS_WRITE_FILE
      | LANDLOCK_ACCESS_FS_MAKE_REG
      | LANDLOCK_ACCESS_FS_REMOVE_FILE
    );
}

void landlock_apply(int fd)
{
    int r = landlock_restrict_self(fd, 0);
    CHECK(r, "landlock_restrict_self");
}
