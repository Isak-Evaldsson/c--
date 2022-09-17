#include "ast.h"
#include "symbol_table.h"
#include "util.h"
#include <stddef.h>

static void name_analysis_func_def(AST_func_def *func_def,
                                   symbol_table_t *table)
{
    error("Name analysis not yet implemented");
}

// Traverses through the symbol table and performs name analysis
void name_analysis(AST_func_list *ast)
{
    symbol_table_t *global_scope = create_symbol_table();

    for (AST_func_list *func = ast; func != NULL; func = func->next) {
        name_analysis_func_def(func->func_def, global_scope);
    }
}
