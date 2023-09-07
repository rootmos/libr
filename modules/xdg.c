#include "xdg.h"
#include "path.h"
#include "fail.h"

#include <limits.h>
#include <string.h>
#include <stdlib.h>

struct xdg {
    char app[NAME_MAX];

    char* home;
    char* dirs[XDG_KINDS];
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

    free(xdg->home);

    for(int i = 0; i < XDG_KINDS; i++) {
        free(xdg->dirs[i]);
    }

    free(xdg);
}

const char* xdg_home(struct xdg* xdg)
{
    if(!xdg->home) {
        char* home = getenv("HOME");
        if(!home) {
            failwith("unable to resolve HOME variable");
        }

        xdg->home = strdup(home);
        CHECK_MALLOC(xdg->home);
    }

    return xdg->home;
}

static size_t check_path(const char* p)
{
    if(!p) return 0;
    size_t r = strnlen(p, PATH_MAX);
    if(r >= PATH_MAX) return 0;
    if(r == 0) return 0;
    if(p[0] != '/') return 0;
    return r;
}

static size_t append_app(const struct xdg* xdg, char* buf, size_t L, const char* p)
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

const char* xdg_dir(struct xdg* xdg, enum xdg_kind k)
{
    if(!xdg->dirs[k]) {
        char buf[PATH_MAX];
        size_t l;

        const char* v = NULL;
        switch(k) {
            case XDG_DATA: v = "XDG_DATA_HOME"; break;
            case XDG_CONFIG: v = "XDG_CONFIG_HOME"; break;
            case XDG_STATE: v = "XDG_STATE_HOME"; break;
            case XDG_CACHE: v = "XDG_CACHE_HOME"; break;
            default: failwith("not implemented: %d", k);
        }

        const char* e = getenv(v);
        if(check_path(e)) {
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
                default:
                    failwith("not implemented: %d", k);
            }
        }
        if(l >= sizeof(buf)) {
            failwith("buffer overflow");
        }

        append_app(xdg, LIT(buf), buf);

        xdg->dirs[k] = strdup(buf);
        CHECK_MALLOC(xdg->dirs[k]);
    }

    return xdg->dirs[k];
}

const char* xdg_data(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_DATA);
}

const char* xdg_config(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_CONFIG);
}

const char* xdg_state(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_STATE);
}

const char* xdg_cache(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_CACHE);
}

const char* xdg_runtime(struct xdg* xdg)
{
    return xdg_dir(xdg, XDG_RUNTIME);
}

char** xdg_data_dirs(struct xdg* xdg)
{
    const char* e = getenv("XDG_DATA_DIRS");
    if(e == NULL || *e == 0) {
        e = "/usr/local/share:/usr/share";
    }

    struct {
        char* path;
        size_t len;
        void* next;
    }* dirs;

    {
        dirs = alloca(sizeof(*dirs));
        const char* h = xdg_data(xdg);
        dirs->len = strlen(h);
        dirs->path = alloca(dirs->len+1);
        memcpy(dirs->path, h, dirs->len+1);
        dirs->next = NULL;
    }
    typeof(*dirs)** tail = (void*)&dirs->next;

    size_t N = 0, n = 1;
    {
        const size_t L = strlen(e);
        char buf[L+1];
        memcpy(buf, e, L+1);
        size_t a = 0, b = 0;
        do {
            for(; buf[b] != ':' && buf[b] != 0; b++);
            buf[b] = 0;
            char* p = &buf[a];
            if(check_path(p)) {
                char q[PATH_MAX];
                size_t l = append_app(xdg, LIT(q), p);
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

        for(typeof(*dirs)* p = dirs; p != NULL; p = p->next) {
            N += sizeof(char*);
            N += p->len+1;
        }
        N += sizeof(char*); // add space for the NULL guard
    }

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

    return buf;
}
