#include "ast.h"
#include "util.h"

AST_func_list *create_func_list(AST_func_def *def, AST_func_list *next)
{
    AST_func_list *list = xmalloc(sizeof(AST_func_list));
    list->func_def = def;
    list->next = next;
    return list;
}

AST_func_def *create_func_def(AST_prototype *proto, AST_stmt_list *stmts)
{
    AST_func_def *func_decl = xmalloc(sizeof(AST_func_def));
    func_decl->proto = proto;
    func_decl->stmts = stmts;
    return func_decl;
}

AST_prototype *create_prototype_ast(char *identifier, AST_param_list *params)
{
    AST_prototype *proto = xmalloc(sizeof(AST_prototype));
    proto->identifier = identifier;
    proto->params = params;
    return proto;
}

AST_param_list *create_param_list(char *identifier, AST_param_list *next)
{
    AST_param_list *param = xmalloc(sizeof(AST_param_list));
    param->identifier = identifier;
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
        stmt->stmt.var_decl.identifier = identifier;
        break;

    default:
        error("Invalid stmt_type %d", type);
        break;
    }
    return stmt;
}

AST_expr *create_expr(expr_type type, int literal)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));

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

void print_proto(AST_prototype *proto, FILE *fp)
{
    AST_param_list *param;

    fprintf(fp, "Prototype: %s\n", proto->identifier);

    for (param = proto->params; param != NULL; param = param->next) {
        for (size_t i = 0; i < INDENT; i++)
            fputc(' ', fp);

        fprintf(fp, "Param: %s\n", param->identifier);
    }
}

void print_func_list(AST_func_list *list, FILE *fp)
{
    print_proto(list->func_def->proto, fp);
    print_stmt_list(list->func_def->stmts, 1, fp);
    if (list->next)
        print_func_list(list->next, fp);
}

void print_stmt_list(AST_stmt_list *list, int level, FILE *fp)
{
    AST_stmt_list *entry;

    for (entry = list; entry != NULL; entry = entry->next) {
        print_stmt(entry->stmt, level, fp);
    }
}

void print_stmt(AST_stmt *stmt, int level, FILE *fp)
{
    for (size_t i = 0; i < level * INDENT; i++)
        fputc(' ', fp);

    switch (stmt->type) {
    case STMT_VAR_DECL:
        fprintf(fp, "VarDecl: %s\n", stmt->stmt.var_decl.identifier);

        if (stmt->stmt.var_decl.expr) {
            print_expr(stmt->stmt.var_decl.expr, level + 1, fp);
        }
        break;
    default:
        fprintf(fp, "UknownStmt: %d\n", stmt->type);
        break;
    }
}

void print_expr(AST_expr *expr, int level, FILE *fp)
{
    for (size_t i = 0; i < level * INDENT; i++)
        fputc(' ', fp);

    switch (expr->type) {
    case EXPR_LITERAL:
        fprintf(fp, "LiteralExpr: %d\n", expr->expr.lit);
        break;
    default:
        fprintf(fp, "UknownExpr: %d\n", expr->type);
        break;
    }
}