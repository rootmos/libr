#include "xdg.h"
#include "fail.h"
#include "path.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

struct xdg {
    char app[NAME_MAX];

    char* dir[XDG_KINDS];
    char** dirs[XDG_KINDS];
};

struct xdg* xdg_new(const char* app)
{
    struct xdg* xdg = calloc(1, sizeof(*xdg));
    CHECK_MALLOC(xdg);

    if(app) {
        const size_t N = sizeof(xdg->app);
        size_t n = strnlen(app, N);
        if(n >= N) {
            failwith("application name truncated at %zu characters", n);
        }

        if(n > 0) {
            memcpy(xdg->app, app, n);
        }
    }

    return xdg;
}

void xdg_free(struct xdg* xdg)
{
    if(xdg == NULL) return;

    for(int i = 0; i < XDG_KINDS; i++) {
        free(xdg->dir[i]);
        free(xdg->dirs[i]);
    }

    free(xdg);
}

static size_t xdg_check_path(const char* p)
{
    if(!p) return 0;
    size_t r = strnlen(p, PATH_MAX);
    if(r >= PATH_MAX) return 0;
    if(r == 0) return 0;
    if(p[0] != '/') return 0;
    return r;
}

static size_t xdg_append_app(const struct xdg* xdg, char* buf, size_t L, const char* p)
{
    if(xdg->app[0]) {
        size_t l = path_join(buf, L, p, xdg->app, NULL);
        if(l >= L) {
            failwith("buffer overflow");
        }

        return l;
    } else {
        return strlen(buf);
    }
}

static size_t xdg_fallback_runtime_dir(char* buf, size_t L)
{
    char template[NAME_MAX];
    size_t n = snprintf(LIT(template), "/tmp/xdg-runtime-fallback-%d-XXXXXX", geteuid());
    if(n >= sizeof(template)) {
        failwith("buffer overflow");
    }

    // mkdtemp(3): "The directory is then created with permissions 0700."
    char* tmp = mkdtemp(template);
    CHECK_NOT(tmp, NULL, "mkdtemp(%s)", template);
    warning("using fallback directory for an unset XDG_RUNTIME_DIR: %s", tmp);
    return snprintf(buf, L, "%s", tmp);
}

const char* xdg_dir(struct xdg* xdg, enum xdg_kind k)
{
    if(xdg->dir[k]) return xdg->dir[k];

    if(k == XDG_HOME) {
        char* home = getenv("HOME");
        if(!home) {
            failwith("unable to resolve HOME variable");
        }

        xdg->dir[k] = strdup(home);
        CHECK_MALLOC(xdg->dir[k]);
        return xdg->dir[k];
    }

    char buf[PATH_MAX];
    size_t l;

    const char* v = NULL;
    switch(k) {
        case XDG_DATA: v = "XDG_DATA_HOME"; break;
        case XDG_CONFIG: v = "XDG_CONFIG_HOME"; break;
        case XDG_STATE: v = "XDG_STATE_HOME"; break;
        case XDG_CACHE: v = "XDG_CACHE_HOME"; break;
        case XDG_RUNTIME: v = "XDG_RUNTIME_DIR"; break;
        default: failwith("unexpected kind: %d", k);
    }

    const char* e = getenv(v);
    if(xdg_check_path(e)) {
        l = path_join(LIT(buf), e, NULL);
    } else {
        switch(k) {
            case XDG_DATA:
                l = path_join(LIT(buf), xdg_home(xdg), ".local", "share", NULL);
                break;
            case XDG_CONFIG:
                l = path_join(LIT(buf), xdg_home(xdg), ".config", NULL);
                break;
            case XDG_STATE:
                l = path_join(LIT(buf), xdg_home(xdg), ".local", "state", NULL);
                break;
            case XDG_CACHE:
                l = path_join(LIT(buf), xdg_home(xdg), ".cache", NULL);
                break;
            case XDG_RUNTIME:
                l = xdg_fallback_runtime_dir(LIT(buf));
                break;
            default:
                failwith("unexpected kind: %d", k);
        }
    }
    if(l >= sizeof(buf)) {
        failwith("buffer overflow");
    }

    xdg_append_app(xdg, LIT(buf), buf);

    xdg->dir[k] = strdup(buf);
    CHECK_MALLOC(xdg->dir[k]);

    return xdg->dir[k];
}

const char* xdg_home(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_HOME);
}

const char* xdg_data_home(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_DATA);
}

const char* xdg_config_home(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_CONFIG);
}

const char* xdg_state_home(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_STATE);
}

const char* xdg_cache_home(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_CACHE);
}

const char* xdg_runtime(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_RUNTIME);
}

const char** xdg_dirs(struct xdg* xdg, enum xdg_kind k)
{
    if(xdg->dirs[k]) return (const char**)xdg->dirs[k];

    const char* e = NULL;
    if(k == XDG_DATA || k == XDG_CONFIG) {
        const char* v = NULL;
        if(k == XDG_DATA) {
            v = "XDG_DATA_DIRS";
        } else if(k == XDG_CONFIG) {
            v = "XDG_CONFIG_DIRS";
        }

        e = getenv(v);
        if(e == NULL || *e == 0) {
            if(k == XDG_DATA) {
                e = "/usr/local/share:/usr/share";
            } else if(k == XDG_CONFIG) {
                e = "/etc/xdg:/etc";
            }
        }
    }

    struct {
        char* path;
        size_t len;
        void* next;
    }* dirs;

    {
        dirs = alloca(sizeof(*dirs));
        const char* h = xdg_dir(xdg, k);
        dirs->len = strlen(h);
        dirs->path = alloca(dirs->len+1);
        memcpy(dirs->path, h, dirs->len+1);
        dirs->next = NULL;
    }
    typeof(*dirs)** tail = (void*)&dirs->next;

    size_t n = 1;
    if(e) {
        const size_t L = strlen(e);
        char buf[L+1];
        memcpy(buf, e, L+1);
        size_t a = 0, b = 0;
        do {
            for(; buf[b] != ':' && buf[b] != 0; b++);
            buf[b] = 0;
            char* p = &buf[a];
            if(xdg_check_path(p)) {
                char q[PATH_MAX];
                size_t l = xdg_append_app(xdg, LIT(q), p);
                if(l >= sizeof(q)) {
                    failwith("buffer overflow");
                }

                typeof(*dirs)* t = alloca(sizeof(*dirs));
                t->len = l;
                t->path = alloca(l+1);
                memcpy(t->path, q, l+1);
                t->next = NULL;

                *tail = t;
                tail = (void*)&t->next;
                n += 1;
            }
            b += 1;
            a = b;
        } while(b < L);
    }

    size_t N = 0;
    for(typeof(*dirs)* p = dirs; p != NULL; p = p->next) {
        N += sizeof(char*);
        N += p->len+1;
    }
    N += sizeof(char*); // add space for the NULL guard

    void* buf = malloc(N);
    CHECK_MALLOC(buf);
    char** index = buf;

    index[n] = NULL;
    char* strings = (char*)&index[n+1];

    size_t i = 0;
    for(typeof(*dirs)* p = dirs; p != NULL; i++, p = p->next) {
        index[i] = strings;
        memcpy(strings, p->path, p->len + 1);
        strings += p->len + 1;
    }

    xdg->dirs[k] = buf;
    return buf;
}

const char** xdg_data_dirs(struct xdg* xdg)
{
    return xdg_dirs(xdg, XDG_DATA);
}

const char** xdg_config_dirs(struct xdg* xdg)
{
    return xdg_dirs(xdg, XDG_CONFIG);
}

const char* xdg_resolvev(struct xdg* xdg, enum xdg_kind k, char* buf, size_t L, va_list ps)
{
    const char** dirs = xdg_dirs(xdg, k);
    for(size_t i = 0; dirs[i]; i++) {
        va_list qs;
        va_copy(qs, ps);
        size_t l = path_joinv(buf, L, dirs[i], qs);
        if(l >= L) {
            failwith("buffer overflow");
        }
        va_end(qs);

        struct stat st;
        int r = stat(buf, &st);
        if(r == -1 && (errno == EACCES || errno == ENOENT || errno == ENOTDIR)) {
            continue;
        }
        CHECK(r, "stat(%s)", buf);

        return buf;
    }

    return NULL;
}

const char* xdg_resolve(struct xdg* xdg, enum xdg_kind k, char* buf, size_t L, ...)
{
    va_list ps; va_start(ps, L);
    const char* p = xdg_resolvev(xdg, k, buf, L, ps);
    return va_end(ps), p;
}

const char* xdg_resolvevs(struct xdg* xdg, enum xdg_kind k, va_list ps)
{
    static char buf[PATH_MAX];
    return xdg_resolvev(xdg, k, LIT(buf), ps);
}

const char* xdg_resolves(struct xdg* xdg, enum xdg_kind k, ...)
{
    va_list ps; va_start(ps, k);
    const char* p = xdg_resolvevs(xdg, k, ps);
    return va_end(ps), p;
}

void xdg_makedirs(const char* path)
{
    int r = makedirs(path, 0700);
    CHECK(r, "makedirs(%s, 0700)", path);
}

const char* xdg_preparev(struct xdg* xdg, enum xdg_kind k, char* buf, size_t L, va_list ps)
{
    size_t l = path_joinv(buf, L, xdg_dir(xdg, k), ps);
    if(l >= L) {
        failwith("buffer overflow");
    }

    struct stat st;
    int r = stat(buf, &st);
    if(r == -1 && errno == ENOENT) {
        char d[l+1];
        memcpy(d, buf, l+1);
        xdg_makedirs(dirname(d));
        return buf;
    }
    CHECK(r, "stat(%s)", buf);

    return buf;
}

const char* xdg_prepare(struct xdg* xdg, enum xdg_kind k, char* buf, size_t L, ...)
{
    va_list ps; va_start(ps, L);
    const char* p = xdg_preparev(xdg, k, buf, L, ps);
    return va_end(ps), p;
}

const char* xdg_preparevs(struct xdg* xdg, enum xdg_kind k, va_list ps)
{
    static char buf[PATH_MAX];
    return xdg_preparev(xdg, k, LIT(buf), ps);
}

const char* xdg_prepares(struct xdg* xdg, enum xdg_kind k, ...)
{
    va_list ps; va_start(ps, k);
    const char* p = xdg_preparevs(xdg, k, ps);
    return va_end(ps), p;
}
