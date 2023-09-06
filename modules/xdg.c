#include "xdg.h"
#include "path.h"
#include "fail.h"

#include <limits.h>
#include <string.h>
#include <stdlib.h>

struct xdg {
    char app[NAME_MAX];

    char* home;
    char* data_home;
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
    free(xdg->data_home);

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

char* xdg_data_home(struct xdg* xdg, ...)
{
    if(!xdg->data_home) {
        char buf[PATH_MAX];

        size_t l;
        char* e = getenv("XDG_DATA_HOME");
        if(check_path(e)) {
            if(xdg->app[0]) {
                l = path_join(LIT(buf), e, xdg->app, NULL);
            } else {
                l = path_join(LIT(buf), e, NULL);
            }
        } else {
            const char* home = xdg_home(xdg);
            if(xdg->app[0]) {
                l = path_join(LIT(buf), home, ".local", "share", xdg->app, NULL);
            } else {
                l = path_join(LIT(buf), home, ".local", "share", NULL);
            }
        }
        if(l >= sizeof(buf)) {
            failwith("buffer overflow");
        }

        xdg->data_home = strdup(buf);
        CHECK_MALLOC(xdg->data_home);
    }

    return xdg->data_home;
}
