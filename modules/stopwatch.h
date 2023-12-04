#pragma once

#include <stddef.h>

struct stopwatch;

struct stopwatch* LIBR(stopwatch_mk)(const char* const what, const size_t period);
void LIBR(stopwatch_free)(struct stopwatch* const s);

void LIBR(stopwatch_reset)(struct stopwatch* const s);
void LIBR(stopwatch_tick)(struct stopwatch* const s);
void LIBR(stopwatch_start)(struct stopwatch* const s);
void LIBR(stopwatch_stop)(struct stopwatch* const s);
