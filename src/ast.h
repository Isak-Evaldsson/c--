#ifndef AST_HEADER
#define AST_HEADER
#include <stdio.h>

/* Number of spaces for each indent level when printing ast */
#define INDENT 4

/* Typedef Ast nodes */
typedef struct AST_func_list AST_func_list;
typedef struct AST_func_def AST_func_def;
typedef struct AST_prototype AST_prototype;
typedef struct AST_param_list AST_param_list;
typedef struct AST_func_def AST_func_def;
typedef struct AST_stmt_list AST_stmt_list;

typedef enum { PROTOTYPE, FUNC_DECL } decl_type;

/* Ast node definitions */
struct AST_func_list {
    AST_func_def *func_def;
    AST_func_list *next;
};

struct AST_prototype {
    char *identifier;
    AST_param_list *params;
};

struct AST_param_list {
    char *identifier;
    AST_param_list *next;
};

struct AST_func_def {
    AST_prototype *proto;
    AST_stmt_list *stmts;
};

struct AST_stmt_list {
    /* placeholder struct */
};

/* Constructors */
AST_func_list *create_func_list(AST_func_def *def, AST_func_list *next);
AST_func_def *create_func_def(AST_prototype *proto, AST_stmt_list *stmt);
AST_prototype *create_prototype_ast(char *identifier, AST_param_list *params);
AST_param_list *create_param_list(char *identifier, AST_param_list *next);

/* Prints functions */
void print_func_list(AST_func_list *list, FILE *fp);
void print_proto(AST_prototype *func, FILE *fp);

#endif