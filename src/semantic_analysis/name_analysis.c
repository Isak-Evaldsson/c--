#include "ast.h"
#include "error_list.h"
#include "symbol_table.h"
#include "util.h"
#include <stddef.h>

static void name_analysis_expr(symbol_table_t *scope, AST_expr *expr)
{
    switch (expr->type) {
    case EXPR_VAR_USE:
        if (symbol_table_lookup(scope, expr->expr.var) == false) {
            error_list_add(expr->token.line_num, expr->token.line_pos,
                           "Invalid symbol '%s'", symbol_name(expr->expr.var));
        };
        break;

    default:
        error("Name analysis not yet implement for expressions");
        break;
    }
}

static void name_analysis_stmt(symbol_table_t *scope, AST_stmt *stmt)
{
    switch (stmt->type) {
    case STMT_RETURN:
        name_analysis_expr(scope, stmt->stmt.ret_expr);
        break;

    default:
        error("Name analysis not yet implemented from stmt of type %d",
              stmt->type);
        break;
    }
}

static void name_analysis_func_def(symbol_table_t *scope,
                                   AST_func_def *func_def)
{
    AST_stmt_list *stmt;
    AST_param_list *param;
    symbol_table_t *function_scope = symbol_table_push(scope);

    for (param = func_def->proto->params; param != NULL; param = param->next) {
        symbol_table_add(function_scope, param->identifier);
    }

    for (stmt = func_def->stmts; stmt != NULL; stmt = stmt->next) {
        name_analysis_stmt(function_scope, stmt->stmt);
    }
}

// Traverses through the symbol table and performs name analysis
bool name_analysis(AST_root *ast)
{
    symbol_table_t *scope = create_symbol_table();
    ast->global_scope = scope;

    for (AST_func_list *func = ast->functions; func != NULL;
         func = func->next) {
        name_analysis_func_def(scope, func->func_def);
    }

    return error_list_empty();
}
