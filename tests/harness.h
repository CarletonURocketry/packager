/**
 * @file harness.c
 * @brief A simple C test harness made of macros.
 */
#ifndef _HARNESS_H_
#define _HARNESS_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef TOTAL_COUNT
#error "Define TOTAL_COUNT to be a size_t variable name for counting the total number of tests run."
#endif

#ifndef FAIL_COUNT
#error "Define FAIL_COUNT to be a size_t variable name for counting the total number of tests failed."
#endif

#define COLOUR_RED "\x1b[31m"
#define COLOUR_GREEN "\x1b[32m"
#define COLOUR_RESET "\x1b[0m"

/** Log an asserted statement and immediately return false from failing test function if assertion fails.. */
#define LOG_ASSERT(exp)                                                                                                \
    do {                                                                                                               \
        if (!(exp)) {                                                                                                  \
            fprintf(stderr, "%s:%d:" COLOUR_RED " assertion failed!" COLOUR_RESET "\n\t%s \n.", __FILE__, __LINE__,    \
                    #exp);                                                                                             \
            return false;                                                                                              \
        }                                                                                                              \
    } while (0)

/** Call a test case function and monitor for pass or fail. */
#define RUN_TEST(tc)                                                                                                   \
    do {                                                                                                               \
        TOTAL_COUNT++;                                                                                                 \
        if (tc()) {                                                                                                    \
            printf("%s ::" COLOUR_GREEN "[PASSED]" COLOUR_RESET "\n", #tc);                                            \
        } else {                                                                                                       \
            FAIL_COUNT++;                                                                                              \
            fprintf(stderr, "%s :: " COLOUR_RED "[FAILED]\n" COLOUR_RESET, #tc);                                       \
        }                                                                                                              \
    } while (0)

/** Print overall results. */
#define HARNESS_RESULTS()                                                                                              \
    do {                                                                                                               \
        if (FAIL_COUNT == 0) {                                                                                         \
            printf("ALL %lu TESTS PASSED.\n", TOTAL_COUNT);                                                            \
            exit(EXIT_SUCCESS);                                                                                        \
        } else {                                                                                                       \
            fprintf(stderr, "PASSED: %lu\nFAILED: %lu\n", TOTAL_COUNT - FAIL_COUNT, FAIL_COUNT);                       \
            exit(EXIT_FAILURE);                                                                                        \
        }                                                                                                              \
    } while (0)

#endif // _HARNESS_H_
