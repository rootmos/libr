#pragma once

#include <stddef.h>
#include <stdarg.h>

// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html

struct xdg;

enum xdg_kind {
    XDG_HOME = 0,
    XDG_DATA,
    XDG_CONFIG,
    XDG_STATE,
    XDG_CACHE,
    XDG_RUNTIME,
    XDG_KINDS,
};

struct xdg* LIBR(xdg_new)(const char* app);
void LIBR(xdg_free)(struct xdg* xdg);

const char* LIBR(xdg_dir)(struct xdg* xdg, enum xdg_kind k);
const char** LIBR(xdg_dirs)(struct xdg* xdg, enum xdg_kind k);

const char* LIBR(xdg_home)(struct xdg* xdg);
const char* LIBR(xdg_data_home)(struct xdg* xdg);
const char* LIBR(xdg_config_home)(struct xdg* xdg);
const char* LIBR(xdg_state_home)(struct xdg* xdg);
const char* LIBR(xdg_cache_home)(struct xdg* xdg);
const char* LIBR(xdg_runtime)(struct xdg* xdg);

const char** LIBR(xdg_data_dirs)(struct xdg* xdg);
const char** LIBR(xdg_config_dirs)(struct xdg* xdg);

const char* LIBR(xdg_resolve)(struct xdg* xdg, enum xdg_kind k, char* buf, size_t L, ...);
const char* LIBR(xdg_resolvev)(struct xdg* xdg, enum xdg_kind k, char* buf, size_t L, va_list ps);
const char* LIBR(xdg_resolves)(struct xdg* xdg, enum xdg_kind k, ...);
const char* LIBR(xdg_resolvevs)(struct xdg* xdg, enum xdg_kind k, va_list ps);

const char* LIBR(xdg_preparev)(struct xdg* xdg, enum xdg_kind k, char* buf, size_t L, va_list ps);
const char* LIBR(xdg_prepare)(struct xdg* xdg, enum xdg_kind k, char* buf, size_t L, ...);
const char* LIBR(xdg_preparevs)(struct xdg* xdg, enum xdg_kind k, va_list ps);
const char* LIBR(xdg_prepares)(struct xdg* xdg, enum xdg_kind k, ...);
