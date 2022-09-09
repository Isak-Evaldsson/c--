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

AST_expr *create_literal_expr(expr_type type, int literal)
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

AST_expr *create_binary_expr(binop_type type, AST_expr *left, AST_expr *right)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));

    expr->type = EXPR_BIN_OP;
    expr->expr.binop.op = type;
    expr->expr.binop.left = left;
    expr->expr.binop.right = right;

    return expr;
}

AST_expr *create_var_use_expr(char *var)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));

    expr->type = EXPR_VAR_USE;
    expr->expr.var = var;
    return expr;
};

AST_expr *create_neg_expr(AST_expr *unary)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));

    expr->type = EXPR_NEG;
    expr->expr.unary = unary;
    return expr;
}

AST_expr *create_func_call_expr(char *identifier, AST_expr_list *args)
{
    AST_expr *expr = xmalloc(sizeof(AST_expr));
    AST_func_call *call = xmalloc(sizeof(AST_func_call));

    call->identifier = identifier;
    call->args = args;

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

    free(proto->identifier);
    free(proto);
}

void free_param_list(AST_param_list *param)
{
    if (param->next)
        free_param_list(param->next);

    free(param->identifier);
    free(param);
}

void free_stmt_list(AST_stmt_list *list)
{
    if (list->next)
        free_stmt_list(list->next);

    free_stmt(list->stmt);
    free(list);
}

void free_stmt(AST_stmt *stmt)
{
    if (stmt->type == STMT_VAR_DECL) {
        free(stmt->stmt.var_decl.identifier);

        if (stmt->stmt.var_decl.expr)
            free_expr(stmt->stmt.var_decl.expr);
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

    free(call->identifier);
    free(call);
}

void free_expr(AST_expr *expr)
{
    switch (expr->type) {
    case EXPR_BIN_OP:
        free_expr(expr->expr.binop.left);
        free_expr(expr->expr.binop.right);
        break;

    case EXPR_VAR_USE:
        free(expr->expr.var);
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

void print_expr_list(AST_expr_list *list, int level, FILE *fp)
{
    AST_expr_list *entry;

    for (entry = list; entry != NULL; entry = entry->next) {
        print_expr(entry->expr, level, fp);
    }
}

// Helper function converting a binop enum to a string representation
static const char *binop2string(binop_type op)
{
    switch (op) {
    case BINOP_PLUS:
        return "+";

    case BINOP_MINUS:
        return "-";

    case BINOP_DIV:
        return "/";

    case BINOP_MUL:
        return "*";

    default:
        return "INVALID OPERATOR";
    }
}

void print_expr(AST_expr *expr, int level, FILE *fp)
{
    const char *op_str;

    for (size_t i = 0; i < level * INDENT; i++)
        fputc(' ', fp);

    switch (expr->type) {
    case EXPR_LITERAL:
        fprintf(fp, "LiteralExpr: %d\n", expr->expr.lit);
        break;

    case EXPR_BIN_OP:
        op_str = binop2string(expr->expr.binop.op);

        fprintf(fp, "BinOpExpr: %s\n", op_str);
        print_expr(expr->expr.binop.left, level + 1, fp);
        print_expr(expr->expr.binop.right, level + 1, fp);
        break;

    case EXPR_VAR_USE:
        fprintf(fp, "VarUseExpr: %s\n", expr->expr.var);
        break;

    case EXPR_NEG:
        fprintf(fp, "NegationExpr: \n");
        print_expr(expr->expr.unary, level + 1, fp);
        break;

    case EXPR_FUNC_CALL:
        fprintf(fp, "FunCallExpr: %s\n", expr->expr.call->identifier);
        print_expr_list(expr->expr.call->args, level + 1, fp);
        break;

    default:
        fprintf(fp, "UknownExpr: %d\n", expr->type);
        break;
    }
}