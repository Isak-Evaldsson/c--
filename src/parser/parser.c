#include "parser.h"
#include "error_list.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>

extern AST_func_list *ast_root;

// Flex internals
extern FILE *yyin;
extern void yylex_destroy();

void set_token_data(token_data_t *token)
{
    token->line_num = line_num;
    token->line_pos = token_pos;
};

AST_root *parse(char *file_name)
{
    error_list_init(file_name);

    /* opening file */
    yyin = fopen(file_name, "r");
    if (!yyin) {
        error_list_add(0, 0, "cannot open file");
        return NULL;
    }

    int error = yyparse();

    // Flex cleanup
    fclose(yyin);
    yylex_destroy();

    return (error == 0) ? create_ast_root(ast_root) : NULL;
}