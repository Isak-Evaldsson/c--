#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include "parser.h"
#include "lexer.h"

int main(int argc, char **argv)
{
    DIR *FD;
    struct dirent *in_file;
    FILE *entry_file;
    FILE *out_file;

    char file_path[500];
    char diff_comand[500] = "diff ";
    int n = 0;
    int n_succ = 0;

    if (argc <= 1)
    {
        printf("Usage: ./tester 'parse-tests-path'");
        return 1;
    }
    printf("Perfroming unit test\n");
    printf("--------------------\n");
    printf("Parse tests\n");

    // Reads parse test folder
    if (NULL == (FD = opendir(argv[1])))
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        return 1;
    }

    // Go through test files and try to parse
    while ((in_file = readdir(FD)))
    {
        // Only read in file
        if (strstr(in_file->d_name, ".in") == NULL)
            continue;

        // Writes correct path
        strcpy(file_path, argv[1]);
        strcat(file_path, "/");
        strcat(file_path, in_file->d_name);

        entry_file = fopen(file_path, "rw");
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
            return 1;
        }

        init_lexer(entry_file);
        expr_ast *expr = parse();
        fclose(entry_file);

        if (expr != NULL)
        {
            // Create out-file
            size_t length = strlen(file_path);
            file_path[length - 3] = '\0';
            strcat(file_path, ".out");
            out_file = fopen(file_path, "w+");

            // Write ast to out-file
            print(expr, 0, out_file);
            fclose(out_file);

            // Building command 'diff out_file expected_file'
            strcat(diff_comand, file_path);
            strcat(diff_comand, " ");
            file_path[length - 3] = '\0';
            strcat(file_path, ".expected");
            strcat(diff_comand, file_path);

            if (system(diff_comand) == 0)
            {
                n_succ++;
                printf("%s - Passed\n", in_file->d_name);
            }
            else
            {
                printf("%s - Failed\n", in_file->d_name);
            }
            diff_comand[5] = '\0';
        }
        else
        {
            printf("%s - Failed\n", in_file->d_name);
        }

        n++;
    }

    if (n_succ == n)
    {
        printf("Passed all the tests");
        return 0;
    }
    else
    {
        printf("%d of %d tests passed\n", n_succ, n);
        return 1;
    }
}