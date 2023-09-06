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

        if(xdg->app[0]) {
            l = path_join(LIT(buf), buf, xdg->app, NULL);
        }
        if(l >= sizeof(buf)) {
            failwith("buffer overflow");
        }

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

    return NULL;
}
