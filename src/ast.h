#ifndef AST_HEADER
#define AST_HEADER
#include <stdio.h>

/* Number of spaces for each indent level when printing ast */
#define INDENT 4

/* Typedef Ast nodes */
typedef struct AST_prototype AST_prototype;
typedef struct AST_param_list AST_param_list;

/* Ast node definitions */
struct AST_prototype {
    char *identifier;
    AST_param_list *params;
};

struct AST_param_list {
    char *identifier;
    AST_param_list *next;
};

/* Constructors */
AST_prototype *create_prototype_ast(char *identifier, AST_param_list *params);
AST_param_list *create_param_list(char *identifier, AST_param_list *next);

/* Prints functions */
void print_proto(AST_prototype *func, FILE *fp);

#endif