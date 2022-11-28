#include "error_list.h"
#include "parser.h"
#include "semantic_analysis.h"
#include "test_framework.h"

static void parse_test(FILE *out_file, char *path)
{
    AST_root *ast = parse(path);

    if (ast == NULL) {
        fprintf(out_file, "PARSING ERROR for %s\n", path);
        return;
    }

    print_ast(ast, out_file);

    // Make sure there's no memory leaks
    free_ast_root(ast);
    free_symbols();
}

static void name_analysis_test(FILE *out_file, char *path)
{
    AST_root *ast = parse(path);

    if (ast == NULL) {
        fprintf(out_file, "PARSING ERROR for %s\n", path);
        return;
    }

    if (!name_analysis(ast)) {
        error_list_print(out_file, false);
    };

    // Make sure theres no memory leaks
    free_ast_root(ast);
    free_symbols();
}

int main(int argc, char **argv)
{
    test_suite_t tests[] = {
        { .path = "files/parser", .name = "parser tests", .test = parse_test },
        { .path = "files/name_analysis",
          .name = "name analysis tests",
          .test = name_analysis_test },
    };

    if (argc <= 1) {
        printf("Usage: %s 'tests-path'", argv[0]);
        return 1;
    }

    return run_suite(argv[1], tests, sizeof(tests) / sizeof(test_suite_t));
}
