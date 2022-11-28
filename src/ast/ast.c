#include "ast.h"
#include "parser.h"
#include "util.h"

AST_root *create_ast_root(AST_func_list *functions)
{
    AST_root *root = xmalloc(sizeof(AST_root));
    root->functions = functions;
    root->global_scope = NULL;
    return root;
}

AST_func_list *create_func_list(AST_func_def *def, AST_func_list *next)
{
    AST_func_list *list = xmalloc(sizeof(AST_func_list));
    list->func_def = def;
    list->next = next;
    return list;
}

AST_func_def *create_func_def(AST_prototype *proto, AST_stmt_list *stmts)
{
    AST_func_def *func_def = xmalloc(sizeof(AST_func_def));
    func_def->proto = proto;
    func_def->stmts = stmts;
    return func_def;
}

AST_prototype *create_prototype_ast(char *identifier, AST_param_list *params)
{
    AST_prototype *proto = xmalloc(sizeof(AST_prototype));
    proto->identifier = get_symbol(identifier);
    proto->params = params;
    return proto;
}

AST_param_list *create_param_list(char *identifier, AST_param_list *next)
{
    AST_param_list *param = xmalloc(sizeof(AST_param_list));
    param->identifier = get_symbol(identifier);
    param->next = next;
    return param;
}

AST_stmt_list *create_stmt_list(AST_stmt *stmt, AST_stmt_list *next)
{
    AST_stmt_list *list = xmalloc(sizeof(AST_stmt_list));
    list->stmt = stmt;
    list->next = next;
    return list;
}

AST_stmt *create_stmt(stmt_type type, char *identifier, AST_expr *expr)
{
    AST_stmt *stmt = xmalloc(sizeof(AST_stmt));

    switch (stmt->type = type) {
    case STMT_VAR_DECL:
        stmt->stmt.var_decl.expr = expr;
        stmt->stmt.var_decl.identifier = get_symbol(identifier);
        break;
    case STMT_ASSIGN:
        stmt->stmt.var_decl.expr = expr;
        stmt->stmt.var_decl.identifier = get_symbol(identifier);
        break;

    case STMT_RETURN:
        stmt->stmt.ret_expr = expr;
        break;

    default:
        error("Invalid stmt_type %d", type);
        break;
    }
    return stmt;
}

AST_stmt *create_while_stmt(AST_expr *pred, AST_stmt_list *block)
{
    AST_stmt *stmt = xmalloc(sizeof(AST_stmt));

    stmt->type = STMT_WHILE;
    stmt->stmt.loop.pred = pred;
    stmt->stmt.loop.block = block;

    return stmt;
}

AST_stmt *create_func_call_stmt(AST_func_call *call)
{
    AST_stmt *stmt = xmalloc(sizeof(AST_stmt));

    stmt->stmt.func_call = call;
    stmt->type = STMT_FUNC_CALL;

    return stmt;
}

AST_stmt *create_if_stmt(AST_expr *pred, AST_stmt_list *block,
                         AST_stmt_list *else_block)
{
    AST_stmt *stmt = xmalloc(sizeof(AST_stmt));

    stmt->type = STMT_IF;
    stmt->stmt.if_stmt.pred = pred;
    stmt->stmt.if_stmt.block = block;
    stmt->stmt.if_stmt.else_block = else_block;

    return stmt;
}

AST_expr *create_literal_expr(expr_type type, int literal)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));
    set_token_data(&expr->token);

    switch (expr->type = type) {
    case EXPR_LITERAL:
        expr->expr.lit = literal;
        break;

    default:
        error("Invalid expr_type %d", type);
        break;
    }
    return expr;
}

AST_expr *create_binary_expr(binop_type type, AST_expr *left, AST_expr *right)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));
    set_token_data(&left->token);

    expr->type = EXPR_BIN_OP;
    expr->expr.binop.op = type;
    expr->expr.binop.left = left;
    expr->expr.binop.right = right;

    return expr;
}

AST_expr *create_var_use_expr(char *var)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));
    set_token_data(&expr->token);

    expr->type = EXPR_VAR_USE;
    expr->expr.var = get_symbol(var);
    return expr;
};

AST_expr *create_neg_expr(AST_expr *unary)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));
    set_token_data(&unary->token);

    expr->type = EXPR_NEG;
    expr->expr.unary = unary;
    return expr;
}

AST_func_call *create_func_call(char *identifier, AST_expr_list *args)
{
    AST_func_call *call = xmalloc(sizeof(AST_func_call));

    call->identifier = get_symbol(identifier);
    call->args = args;

    return call;
}

AST_expr *create_func_call_expr(AST_func_call *call)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));
    set_token_data(&expr->token);

    expr->type = EXPR_FUNC_CALL;
    expr->expr.call = call;

    return expr;
}

AST_expr_list *create_expr_list(AST_expr *expr, AST_expr_list *next)
{
    AST_expr_list *list = xmalloc(sizeof(AST_expr_list));

    list->expr = expr;
    list->next = next;
    return list;
}

void free_ast_root(AST_root *root)
{
    free_func_list(root->functions);
    symbol_table_free(root->global_scope);
    free(root);
}

void free_func_list(AST_func_list *list)
{
    if (list->next)
        free_func_list(list->next);

    free_func_def(list->func_def);
    free(list);
}

void free_func_def(AST_func_def *def)
{
    if (def->stmts)
        free_stmt_list(def->stmts);

    free_prototype(def->proto);
    free(def);
}

void free_prototype(AST_prototype *proto)
{
    if (proto->params)
        free_param_list(proto->params);

    free(proto);
}

void free_param_list(AST_param_list *param)
{
    if (param->next)
        free_param_list(param->next);

    free(param);
}

void free_stmt_list(AST_stmt_list *list)
{
    if (list == NULL)
        return;

    if (list->next)
        free_stmt_list(list->next);

    free_stmt(list->stmt);
    free(list);
}

void free_stmt(AST_stmt *stmt)
{
    switch (stmt->type) {
    case STMT_VAR_DECL:
    case STMT_ASSIGN:
        if (stmt->stmt.var_decl.expr)
            free_expr(stmt->stmt.var_decl.expr);
        break;

    case STMT_FUNC_CALL:
        free_func_call(stmt->stmt.func_call);
        break;

    case STMT_RETURN:
        free_expr(stmt->stmt.ret_expr);
        break;

    case STMT_WHILE:
        free_expr(stmt->stmt.loop.pred);
        free_stmt_list(stmt->stmt.loop.block);
        break;

    case STMT_IF:
        free_expr(stmt->stmt.if_stmt.pred);
        free_stmt_list(stmt->stmt.if_stmt.block);
        free_stmt_list(stmt->stmt.if_stmt.else_block);
        break;

    default:
        break;
    }

    free(stmt);
}

void free_expr_list(AST_expr_list *list)
{
    if (list->next)
        free_expr_list(list->next);

    free_expr(list->expr);
    free(list);
}

void free_func_call(AST_func_call *call)
{
    if (call->args)
        free_expr_list(call->args);

    free(call);
}

void free_expr(AST_expr *expr)
{
    switch (expr->type) {
    case EXPR_BIN_OP:
        free_expr(expr->expr.binop.left);
        free_expr(expr->expr.binop.right);
        break;

    case EXPR_NEG:
        free_expr(expr->expr.unary);
        break;

    case EXPR_FUNC_CALL:
        free_func_call(expr->expr.call);
        break;

    default:
        /* No alloced members */
        break;
    }
    free(expr);
}
