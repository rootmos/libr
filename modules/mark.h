#pragma once

#include <stddef.h>

struct mark* LIBR(mark_init)(
    const char* const what,
    const double factor,
    const char* const unit,
    const size_t check_period);
void LIBR(mark_free)(struct mark* const m);

void LIBR(mark_tick)(struct mark* const m);
void LIBR(mark_set)(struct mark* const m);
