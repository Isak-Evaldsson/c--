#include <stdio.h>

#include "ast.h"
#include "errormsg.h"
#include "parser.h"
#include "util.h"

int main(int argc, char **argv)
{
    char *file_name;
    AST_func_list *ast;

    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    file_name = argv[1];

    if (!(ast = parse(file_name))) {
        exit(1);
    }

    printf("Compilation successful\n");
    print_func_list(ast, stdout);

    free_func_list(ast);
    EM_free();
    return 0;
}