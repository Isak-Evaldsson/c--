#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "ast.h"
#include <stdio.h>

/* Number of spaces for each indent level when printing ast */
#define INDENT 4

void print_ast(AST_root *ast, FILE *fp);

#endif /* PRINT_AST_H */
