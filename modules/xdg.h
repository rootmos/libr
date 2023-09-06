#pragma once

#include <stddef.h>

// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html

struct xdg;

enum xdg_kind {
    XDG_DATA = 0,
    XDG_CONFIG,
    XDG_STATE,
    XDG_CACHE,
    XDG_RUNTIME,
    XDG_KINDS,
};

struct xdg* xdg_new(const char* app);
void xdg_free(struct xdg* xdg);

const char* xdg_home(struct xdg* xdg);

const char* xdg_dir(struct xdg* xdg, enum xdg_kind k);

const char* xdg_data(struct xdg* xdg);
const char* xdg_config(struct xdg* xdg);
const char* xdg_state(struct xdg* xdg);
const char* xdg_cache(struct xdg* xdg);
const char* xdg_runtime(struct xdg* xdg);

char** xdg_data_dirs(struct xdg* xdg);
