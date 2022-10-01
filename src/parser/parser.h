#include "ast.h"
#include "print_ast.h"

/* Struct containing token information */
typedef struct token_data_t token_data_t;

struct token_data_t {
    int line_num;
    int line_pos;
};

/* File location from lexer */
extern int line_num;
extern int line_pos;
extern int token_pos;

/**
 *  Wrapper function around the bison parser.
 *  @returns Pointer to the ast root node, if parsing fails NULL is returned
 */
AST_func_list *parse(char *file_name);