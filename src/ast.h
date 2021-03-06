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
typedef struct AST_stmt AST_stmt;
typedef struct AST_expr AST_expr;

typedef enum { STMT_VAR_DECL } stmt_type;
typedef enum { EXPR_LITERAL } expr_type;

/* Ast node definitions */
struct AST_func_list {
    AST_func_def *func_def;
    AST_func_list *next;
};

struct AST_func_def {
    AST_prototype *proto;
    AST_stmt_list *stmts;
};

struct AST_prototype {
    char *identifier;
    AST_param_list *params;
};

struct AST_param_list {
    char *identifier;
    AST_param_list *next;
};

struct AST_stmt_list {
    AST_stmt *stmt;
    AST_stmt_list *next;
};

struct AST_stmt {
    stmt_type type;
    union {
        struct {
            char *identifier;
            AST_expr *expr;
        } var_decl;
    } stmt;
};

struct AST_expr {
    expr_type type;
    union {
        int lit;
    } expr;
};

/* Constructors */
AST_func_list *create_func_list(AST_func_def *def, AST_func_list *next);
AST_func_def *create_func_def(AST_prototype *proto, AST_stmt_list *stmt);
AST_prototype *create_prototype_ast(char *identifier, AST_param_list *params);
AST_param_list *create_param_list(char *identifier, AST_param_list *next);
AST_stmt_list *create_stmt_list(AST_stmt *stmt, AST_stmt_list *next);
AST_stmt *create_stmt(stmt_type type, char *identifier, AST_expr *expr);
AST_expr *create_expr(expr_type type, int literal);

/* Destructors */
void free_func_list(AST_func_list *list);
void free_func_def(AST_func_def *def);
void free_prototype(AST_prototype *proto);
void free_param_list(AST_param_list *param);
void free_stmt_list(AST_stmt_list *list);
void free_stmt(AST_stmt *stmt);
void free_expr(AST_expr *expr);

/* Prints functions */
void print_func_list(AST_func_list *list, FILE *fp);
void print_proto(AST_prototype *func, FILE *fp);
void print_stmt_list(AST_stmt_list *list, int level, FILE *fp);
void print_stmt(AST_stmt *stmt, int level, FILE *fp);
void print_expr(AST_expr *expr, int level, FILE *fp);

#endif