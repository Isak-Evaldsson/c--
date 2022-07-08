#include "parser.h"
#include "errormsg.h"
#include "parser.tab.h"

extern AST_prototype* ast_root;

AST_prototype* parse(char* file_name)
{
    EM_reset(file_name);
    if (yyparse() == 0)
        return ast_root;

    return NULL;
}