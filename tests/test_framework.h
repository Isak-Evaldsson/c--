#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H
#include "stddef.h"
#include <stdio.h>

/* Test function, taking an in-data path, and a out file to run the result to,
   the success of the test is determined by comparing the expected file with
   the out file
*/
typedef void (*test_function_t)(FILE *out_file, char *path);

/*  Struct defining all necessary data for a test suite to be run */
typedef struct test_suite_t {
    const char *path;
    const char *name; /* Test name */
    test_function_t test; /* The test function */
} test_suite_t;

/* Runs a list of test suites, returns 1 on failure, else 0 */
int run_suite(char *root_folder, test_suite_t *suite, size_t n);

#endif /* TEST_FRAMEWORK_H */
