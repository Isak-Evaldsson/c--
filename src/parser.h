#include "ast.h"

/**
 *  Wrapper function around the bison parser.
 *  @returns Pointer to the ast root node, if parsing fails NULL is returned
 */
AST_prototype* parse(char* file_name);