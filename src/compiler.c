#include <stdio.h>

#include "ast.h"
#include "errormsg.h"
#include "parser.h"
#include "print_ast.h"
#include "util.h"

void print_help(const char *prog_name)
{
    fprintf(
        stderr,
        "Usage: %s <options> file\n"
        "Option:\n"
        "  -d <operation> Allows the compiler to perform debug operations:\n"
        "     ast - dumps the abstract syntax tree after successful parsing\n"
        "     analysis - exits compilation after performing semantic "
        "analysis\n",
        prog_name);
}

int main(int argc, char **argv)
{
    char *file_name;
    AST_func_list *ast;

    if (argc != 2) {
        print_help(argv[0]);
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