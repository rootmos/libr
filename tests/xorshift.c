#include <r.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <gsl/gsl_cdf.h>

static void uniformly_distributed(uint64_t (*f)(void))
{
    const size_t B = 100000, N = 10000;
    size_t boxes[B]; memset(boxes, 0, sizeof(boxes));

    for(size_t i = 0; i < B; i++) {
        assert(boxes[i] == 0);
    }

    for(size_t i = 0; i < N; i++) {
        boxes[f() % B] += 1;
    }

    double chi2 = 0;
    for(size_t i = 0; i < B; i++) {
        const double e = (double)N/B;
        chi2 += pow(boxes[i] - e, 2)/e;
    }

    assert(gsl_cdf_chisq_Q(chi2, B-1) > 0.001);
}

void xorshift_tests(void)
{
    xorshift_state_initalize();

    uniformly_distributed(xorshift64_i);
    uniformly_distributed(xorshift128plus_i);
}
