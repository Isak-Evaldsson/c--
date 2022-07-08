#include "parser.h"
#include "errormsg.h"
#include "parser.tab.h"

extern AST_func_list *ast_root;

AST_func_list *parse(char *file_name)
{
    EM_reset(file_name);
    if (yyparse() == 0)
        return ast_root;

    return NULL;
}