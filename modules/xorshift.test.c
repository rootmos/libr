#include <assert.h>
#include <math.h>
#include <gsl/gsl_cdf.h>

static void uniformly_distributed(uint64_t (*f)(void))
{
    const size_t B = 10, N = 100000;
    size_t boxes[B]; memset(boxes, 0, sizeof(boxes));

    for(size_t i = 0; i < N; i++) {
        boxes[f() % B] += 1;
    }

    double chi2 = 0;
    for(size_t i = 0; i < B; i++) {
        const double E = (double)N/B;
        const double err = (double)boxes[i] - E;
        chi2 += err*err/E;
    }

    assert(chi2 <= gsl_cdf_chisq_Pinv(0.95, B-1));
}

static void normally_distributed(float (*f)(void))
{
    const size_t B = 5, N = 1000;
    size_t boxes[B+1]; memset(boxes, 0, sizeof(boxes));

    float stddev = 2;
    for(size_t i = 0; i < N; i++) {
        size_t j = floor(fabsf(f()) * stddev);
        boxes[MIN(j, B)] += 1;
    }

    double E = 2*N*gsl_cdf_gaussian_Q(B, stddev);
    double err = (double)boxes[B] - E;
    double chi2 = err*err/E;
    for(size_t i = 0; i < B; i++) {
        E = N * 2 * (gsl_cdf_gaussian_P(i+1, stddev) - gsl_cdf_gaussian_P(i, stddev));
        err = (double)boxes[i] - E;
        chi2 += err*err/E;
    }

    assert(chi2 <= gsl_cdf_chisq_Pinv(0.95, B));
}

TEST_SUITE(xorshift, {
    LIBR(xorshift_state_initialize)();

    TEST(xorshift64_is_uniform, {
         uniformly_distributed(LIBR(xorshift64_i));
    });

    TEST(xorshift128plus_is_uniform, {
          uniformly_distributed(LIBR(xorshift128plus_i));
    });

    TEST(normal_dist_is_normal, {
          normally_distributed(LIBR(normal_dist_i));
    });

    TEST_ABORT(xorshift64_is_not_normal, {
        float f() { return LIBR(uniform_float)(LIBR(xorshift64_i)()); }
        normally_distributed(f);
    });

    TEST_ABORT(normal_dist_is_not_uniform, {
        uint64_t f() { return llrint(fabsf(LIBR(normal_dist_i)())); }
        uniformly_distributed(f);
    });

    TEST_ABORT(normal_dist_test_should_respect_stddev, {
        float f() { return 2*LIBR(normal_dist_i)(); }
        normally_distributed(f);
    });
})
