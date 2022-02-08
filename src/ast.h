#ifndef AST_HEADER
#define AST_HEADER

// Typedefs
typedef struct prototype_ast prototype_ast;
typedef struct args_ast args_ast;
typedef struct func_ast func_ast;
typedef struct expr_ast expr_ast;

// Struct representing the "prototype" for a function,
// which captures its name, and its argument names (thus implicitly the number
// of arguments the function takes).
struct prototype_ast
{
    char *identifier;
    char **args; // string array of arguments
    size_t n;    // array size
    size_t c;    // array capacity
};

// Struct representing the function definition itself.
struct func_ast
{
    prototype_ast *prototype;
    expr_ast *body;
};

// Strict representing different types of expressions
struct expr_ast
{
    enum
    {
        CONSTANT,
        BINARY,
        VAR_USE,
        CALL,
    } type;
    union
    {
        int value;        // used if type number
        char *identifier; // used if type var_use
        struct            // used for binary operation
        {
            char op;
            expr_ast *left;
            expr_ast *right;

        } bin_exprs;
        struct // used for function call
        {
            char *callee;
            expr_ast *params;
            int nbr_params;
        } call;
    } data;
};

/* Creates expressions of different type */
expr_ast *create_bin_expr(int op_token, expr_ast *left, expr_ast *right);
expr_ast *create_const_expr(int value);
expr_ast *create_var_expr(char *identifier);
expr_ast *create_call_expr(char *callee, expr_ast *params, int nbr_params);

#endif