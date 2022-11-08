#include "test_framework.h"
#include "error_list.h"
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// Helper function for building paths
static void build_path(char *dest, const char *path1, const char *path2)
{
    sprintf(dest, "%s/%s", path1, path2);
}

static int test_runner(char *test_folder, test_function_t test)
{
    DIR *FD;
    struct dirent *in_file;
    FILE *out_file;

    char in_path[500];
    char out_path[504];
    char exp_path[509];

    char diff_comand[1018];
    int n = 0;
    int n_succ = 0;

    // Reads parse test folder
    if (NULL == (FD = opendir(test_folder))) {
        fprintf(stderr, "Error : Failed to open input directory - %s\n",
                strerror(errno));
        return 1;
    }

    // Go through test files and try to parse
    while ((in_file = readdir(FD))) {
        // Only read in file
        if (strstr(in_file->d_name, ".in") == NULL)
            continue;

        // Writes correct path
        build_path(in_path, test_folder, in_file->d_name);

        // Create out and expected paths
        size_t length = strlen(in_path);

        // Temporarily remove extension
        in_path[length - 3] = '\0';

        // Create out and expect
        sprintf(out_path, "%s.out", in_path);
        sprintf(exp_path, "%s.expected", in_path);

        // Re-add extension
        in_path[length - 3] = '.';

        out_file = fopen(out_path, "w+");

        // Run the test
        test(out_file, in_path);
        fclose(out_file);

        // Building command 'diff out_file expected_file'
        sprintf(diff_comand, "diff %s %s", out_path, exp_path);

        if (system(diff_comand) == 0) {
            n_succ++;
            printf("%s - Passed\n", in_file->d_name);
        } else {
            printf("%s - Failed\n", in_file->d_name);
        }
        n++;

        // Test cleanup
        error_list_free();
    }

    closedir(FD);
    printf("%d of %d tests passed\n", n_succ, n);
    return n_succ != n;
}

int run_suite(char *root_folder, test_suite_t *suite, size_t n)
{
    char test_folder[500];

    // Print header
    printf("Starting Unit tests: \n----------------------------\n");

    // Go through all the tests
    for (size_t i = 0; i < n; i++) {
        test_suite_t test = suite[i];

        printf("Running %s: \n", test.name);

        // Run tests for that particular suite
        build_path(test_folder, root_folder, test.path);
        int res = test_runner(test_folder, test.test);
        if (res == 1) {
            printf("Failed test suite: %s \n", test.name);
            return 1;
        };
        putchar('\n');
    }

    printf("Passed all the tests");
    return 0;
}
