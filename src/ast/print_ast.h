#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "ast.h"
#include <stdio.h>

/* Number of spaces for each indent level when printing ast */
#define INDENT 4

/* Print ast functions  */
void print_func_list(AST_func_list *list, FILE *fp);
void print_proto(AST_prototype *func, FILE *fp);
void print_stmt_list(AST_stmt_list *list, int level, FILE *fp);
void print_stmt(AST_stmt *stmt, int level, FILE *fp);
void print_expr_list(AST_expr_list *list, int level, FILE *fp);
void print_expr(AST_expr *expr, int level, FILE *fp);
void free_func_call(AST_func_call *call);

#endif /* PRINT_AST_H */
