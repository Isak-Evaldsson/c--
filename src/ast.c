#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "util.h"
#include "lexer.h"

/* Creates different expressions depending of thief type, ignores used parmeters,
    could be used directly or with the predefined macros */
static expr_ast *create_expr(int type, int value, char op, char *identifier, expr_ast *left, expr_ast *right, expr_ast *parms)
{
    expr_ast *e = calloc(1, sizeof(expr_ast));
    e->type = type;

    switch (type)
    {
    case BINARY:
        e->data.bin_exprs.left = left;
        e->data.bin_exprs.right = right;
        e->data.bin_exprs.op = op;
        break;

    case CONSTANT:
        e->data.value = value;
        break;

    case VAR_USE:
        // Make sure strings are copied
        e->data.identifier = copy_str(identifier);
        break;

    case CALL:
        e->data.call.params = parms;
        e->data.call.nbr_params = value;
        e->data.call.callee = identifier;
        return e;
    }
}

expr_ast *create_bin_expr(int op_token, expr_ast *left, expr_ast *right)
{
    char op;

    switch (op_token)
    {
    case ADD_TOK:
        op = '+';
        break;

    case SUB_TOK:
        op = '-';
        break;

    case MUL_TOK:
        op = '*';
        break;

    case LESS_TOK:
        op = '<';
        break;
    default:
        error("ast error", "invalid op_token type %d", op_token);
        break;
    }

    return create_expr(BINARY, 0, op, NULL, left, right, NULL);
}

expr_ast *create_const_expr(int value)
{
    return create_expr(CONSTANT, value, 0, NULL, NULL, NULL, NULL);
}

expr_ast *create_var_expr(char *identifier)
{
    return create_expr(VAR_USE, 0, 0, identifier, NULL, NULL, NULL);
}

expr_ast *create_call_expr(char *callee, expr_ast *params, int nbr_params)
{
    return create_expr(CALL, nbr_params, 0, callee, NULL, NULL, params);
}