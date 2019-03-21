#pragma once

#include <r/util.h>
#include <r/logging.h>
#include <r/fail.h>
#include <r/mark.h>
#include <r/stopwatch.h>
#include <r/xorshift.h>
#include <r/test.h>

int test_runner(const char* name,
                void (*body)(void),
                int expect_signal,
                int expect_exit_code);
