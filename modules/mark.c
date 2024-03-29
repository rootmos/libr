#include "mark.h"
#include "fail.h"
#include "logging.h"

#include <assert.h>
#include <time.h>
#include <stdlib.h>

struct mark {
    const char* what;
    double factor;
    const char* unit;

    struct timespec t;
    size_t count;
    size_t check_period;
};

API void LIBR(mark_set)(struct mark* const m)
{
    int r = clock_gettime(CLOCK_MONOTONIC_RAW, &m->t);
    CHECK(r, "clock_gettime(CLOCK_MONOTONIC_RAW, ..)");

    m->count = 0;
}

API struct mark* LIBR(mark_init)(
    const char* const what,
    const double factor,
    const char* const unit,
    const size_t check_period)
{
    struct mark* m = (struct mark*)calloc(sizeof(*m), 1); assert(m);
    m->what = what;
    m->factor = factor;
    m->unit = unit;
    m->check_period = check_period;
    mark_set(m);

    return m;
}

API void LIBR(mark_free)(struct mark* const m)
{
    free(m);
}

API void LIBR(mark_tick)(struct mark* const m)
{
    m->count += 1;

    if(m->count == m->check_period) {
        const struct timespec old = m->t;
        mark_set(m);
        const time_t secs = m->t.tv_sec - old.tv_sec;
        const time_t nanos = m->t.tv_nsec - old.tv_nsec;
        const double freq =
            m->check_period/(secs + ((double)nanos)/1000000000) * m->factor;
        info("%s: %f%s", m->what, freq, m->unit);
    }
}
