#ifndef _HARNESS_H_
#define _HARNESS_H_

#include <stdbool.h>
#include <stdio.h>

/** Log an asserted statement and immediately return false from failing test function if assertion fails.. */
#define LOG_ASSERT(exp)                                                                                                \
    do {                                                                                                               \
        if (!(exp)) {                                                                                                  \
            fprintf(stderr, "%s:%d: assertion failed\n\t%s \n.", __FILE__, __LINE__, #exp);                            \
            return false;                                                                                              \
        }                                                                                                              \
    } while (0)

/** Call a test case function and monitor for pass or fail. */
#define RUN_TEST(tc)                                                                                                   \
    do {                                                                                                               \
        if (tc()) {                                                                                                    \
            printf("%s :: [PASSED]\n", #tc);                                                                           \
        } else {                                                                                                       \
            fprintf(stderr, "%s :: [FAILED]\n", #tc);                                                                  \
        }                                                                                                              \
    } while (0)

#endif // _HARNESS_H_
