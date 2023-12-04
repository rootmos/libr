#include "stopwatch.h"
#include "fail.h"
#include "logging.h"

#include <time.h>
#include <stdlib.h>

struct stopwatch {
    const char* what;
    size_t period;

    int running;
    struct timespec t;
    struct timespec acc;
    size_t intervals;
    size_t ticks;
};

API void LIBR(stopwatch_reset)(struct stopwatch* const s)
{
    s->running = 0;
    s->ticks = 0;
    s->intervals = 0;
    s->t = (struct timespec){ 0 };
}

API struct stopwatch* LIBR(stopwatch_mk)(
    const char* const what,
    const size_t period)
{
    struct stopwatch* s = calloc(sizeof(*s), 1);
    CHECK_MALLOC(s);
    s->what = what;
    s->period = period;

    LIBR(stopwatch_reset)(s);
    return s;
}

API void LIBR(stopwatch_free)(struct stopwatch* const s)
{
    free(s);
}


API void LIBR(stopwatch_tick)(struct stopwatch* const s)
{
    if(++s->ticks == s->period)
    {
        const double secs = (double)(s->acc.tv_sec) + ((double)(s->acc.tv_nsec))/1000000000;
        const double avg = secs / s->intervals;
        info("%s: avg=%f", s->what, avg);
        s->acc = (struct timespec){ 0 };
        s->intervals = 0;
        s->ticks = 0;
    }
}

API void LIBR(stopwatch_start)(struct stopwatch* const s)
{
    if(!s->running) {
        int r = clock_gettime(CLOCK_MONOTONIC_RAW, &s->t);
        CHECK(r, "clock_gettime(CLOCK_MONOTONIC_RAW, ..)");

        s->running = 1;
    }
}

API void LIBR(stopwatch_stop)(struct stopwatch* const s)
{
    if(s->running) {
        s->intervals++;
        s->running = 0;

        struct timespec now;
        int r = clock_gettime(CLOCK_MONOTONIC_RAW, &now);
        CHECK(r, "clock_gettime(CLOCK_MONOTONIC_RAW, ..)");

        s->acc.tv_sec += now.tv_sec - s->t.tv_sec;
        s->acc.tv_nsec += now.tv_nsec - s->t.tv_nsec;
    }

    LIBR(stopwatch_tick)(s);
}
