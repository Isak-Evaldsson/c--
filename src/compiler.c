#include "ast.h"
#include "errormsg.h"
#include "parser.h"
#include "print_ast.h"
#include "semantic_analysis.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

#define streq(s1, s2) (strcmp(s1, s2) == 0)

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
    char *debug_flag;
    AST_func_list *ast = NULL;

    if (argc < 2) {
        print_help(argv[0]);
        exit(1);
    }

    // parse debug options
    if (streq(argv[1], "-d")) {
        if (argc < 4) {
            print_help(argv[0]);
            exit(1);
        }
        file_name = argv[3];
        debug_flag = argv[2];

        if (!streq(debug_flag, "ast") && !streq(debug_flag, "analysis"))
            error("invalid debug flag '%s'", debug_flag);

    } else {
        if (argc > 2)
            error("%s: invalid argument %s\n", argv[0], argv[2]);

        file_name = argv[1];
        debug_flag = NULL;
    }

    // parse src file
    if (!(ast = parse(file_name))) {
        exit(1);
    }

    // dump ast if specified
    if (debug_flag != NULL && streq(debug_flag, "ast")) {
        printf("Parsing successful. Dumping ast:\n");
        print_func_list(ast, stdout);
        goto end;
    }

    // perform semantic analysis
    name_analysis(ast);
    printf("Compilation successful\n");

end:
    free_func_list(ast);
    EM_free();
    return 0;
}