#pragma once

#include <stddef.h>

// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html

struct xdg;

struct xdg* xdg_new(const char* app);
void xdg_free(struct xdg* xdg);

char* xdg_data_home(struct xdg* xdg, ...);
