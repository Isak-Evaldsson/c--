#include <stdio.h>
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

    char path[500];
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
        if (!strcmp(in_file->d_name, "."))
            continue;
        if (!strcmp(in_file->d_name, ".."))
            continue;

        // Writes correct path
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, in_file->d_name);

        entry_file = fopen(path, "rw");
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
            return 1;
        }

        // set the lexer to read from stdin
        src_file = entry_file;

        if (parse())
        {
            n_succ++;
            printf("%s - Passed\n", in_file->d_name);
        }
        else
        {
            printf("%s - Failed\n", in_file->d_name);
        }

        n++;
        fclose(entry_file);
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