#include "parser.h"
#include "errormsg.h"
#include "parser.tab.h"

extern AST_func_list *ast_root;

// Flex internals
extern FILE *yyin;
extern void yylex_destroy();

AST_func_list *parse(char *file_name)
{
    EM_reset(file_name);
    int error = yyparse();

    // Flex cleanup
    fclose(yyin);
    yylex_destroy();

    return (error == 0) ? ast_root : NULL;
}