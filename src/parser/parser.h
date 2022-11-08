#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "print_ast.h"

void set_token_data(token_data_t *token);

/* File location from lexer */
extern int line_num;
extern int line_pos;
extern int token_pos;

/**
 *  Wrapper function around the bison parser.
 *  @returns Pointer to the ast root node, if parsing fails NULL is returned
 */
AST_func_list *parse(char *file_name);

#endif /* PARSER_H */
