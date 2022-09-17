#include "print_ast.h"
#include "ast.h"
#include <stdio.h>

void print_proto(AST_prototype *proto, FILE *fp)
{
    AST_param_list *param;

    fprintf(fp, "Prototype: %s\n", symbol_name(proto->identifier));

    for (param = proto->params; param != NULL; param = param->next) {
        for (size_t i = 0; i < INDENT; i++)
            fputc(' ', fp);

        fprintf(fp, "Param: %s\n", symbol_name(param->identifier));
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
        fprintf(fp, "VarDecl: %s\n",
                symbol_name(stmt->stmt.var_decl.identifier));

        if (stmt->stmt.var_decl.expr) {
            print_expr(stmt->stmt.var_decl.expr, level + 1, fp);
        }
        break;

    case STMT_ASSIGN:
        fprintf(fp, "Assignment: %s\n",
                symbol_name(stmt->stmt.var_decl.identifier));
        print_expr(stmt->stmt.var_decl.expr, level + 1, fp);
        break;

    case STMT_FUNC_CALL:
        fprintf(fp, "FunCall: %s\n",
                symbol_name(stmt->stmt.func_call->identifier));
        print_expr_list(stmt->stmt.func_call->args, level + 1, fp);
        break;

    case STMT_RETURN:
        fprintf(fp, "Return \n");
        print_expr(stmt->stmt.ret_expr, level + 1, fp);
        break;

    case STMT_WHILE:
        fprintf(fp, "While:\n");
        print_expr(stmt->stmt.loop.pred, level + 1, fp);

        for (size_t i = 0; i < level * INDENT; i++)
            fputc(' ', fp);

        fprintf(fp, "Do:\n");
        print_stmt_list(stmt->stmt.loop.block, level + 1, fp);
        break;

    case STMT_IF:
        fprintf(fp, "If:\n");
        print_expr(stmt->stmt.if_stmt.pred, level + 1, fp);

        for (size_t i = 0; i < level * INDENT; i++)
            fputc(' ', fp);

        fprintf(fp, "Do:\n");
        print_stmt_list(stmt->stmt.if_stmt.block, level + 1, fp);

        // Print optional else
        if (stmt->stmt.if_stmt.else_block != NULL) {
            for (size_t i = 0; i < level * INDENT; i++)
                fputc(' ', fp);

            fprintf(fp, "Else:\n");
            print_stmt_list(stmt->stmt.if_stmt.else_block, level + 1, fp);
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
        fprintf(fp, "VarUseExpr: %s\n", symbol_name(expr->expr.var));
        break;

    case EXPR_NEG:
        fprintf(fp, "NegationExpr: \n");
        print_expr(expr->expr.unary, level + 1, fp);
        break;

    case EXPR_FUNC_CALL:
        fprintf(fp, "FunCallExpr: %s\n",
                symbol_name(expr->expr.call->identifier));
        print_expr_list(expr->expr.call->args, level + 1, fp);
        break;

    default:
        fprintf(fp, "UknownExpr: %d\n", expr->type);
        break;
    }
}