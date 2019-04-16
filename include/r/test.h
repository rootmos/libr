#pragma once

#include <sys/signal.h>

#define TEST(f, body) { void f(void) { body } if(test_runner(#f, f, -1, 0) != 0) exit(1); } while(0)
#define TEST_ABORT(f, body) { void f(void) { body } if(test_runner(#f, f, SIGABRT, -1) != 0) exit(1); } while(0)

int test_runner(const char* name,
                void (*body)(void),
                int expect_signal,
                int expect_exit_code);
