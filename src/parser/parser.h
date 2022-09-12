#include "ast.h"
#include "print_ast.h"

/**
 *  Wrapper function around the bison parser.
 *  @returns Pointer to the ast root node, if parsing fails NULL is returned
 */
AST_func_list *parse(char *file_name);