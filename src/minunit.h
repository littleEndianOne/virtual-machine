/*
MyMinUnit

Adapted from MinUnit:
http://www.jera.com/techinfo/jtns/jtn002.html
*/

#pragma once

extern int tests_run;
extern int tests_passed;
extern int tests_failed;

/* file: minunit.h */
 #define mu_assert(message, test) do {                                  \
                                if (!(test))                            \
                                {                                       \
                                    return message;                     \
                                }                                       \
                                } while (0)                             \

 #define mu_run_test(test) do { char *message = test();                 \
                                tests_run++;                            \
                                if (message)                            \
                                {                                       \
                                    printf(#test ": FAILED, %s \n", message);            \
                                    tests_failed++;                     \
                                }                                       \
                                else                                    \
                                {                                       \
                                    printf(#test ": PASSED! \n") ;      \
                                    tests_passed++;                     \
                                }                                       \
                                } while (0)

