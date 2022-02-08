#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "ast.h"
#include "util.h"

// Functional macros to convet value of numerical macros to strings
#define str(s) #s
#define xstr(s) str(s)

#define MAX_ARGS 30

/* Re-written parser */

/* Prototypes */
static expr_ast *parse_expr();
static expr_ast *parse_primary();
static expr_ast *parse_ident_expr();
static expr_ast *parse_number();
static expr_ast *parse_paren_expr();
static expr_ast *parse_bin_op(int expr_prec, expr_ast *lhs);

/* Writes parser errors to stderr */
static void *log_parse_error(char *msg)
{
    fprintf(stderr, "Parse error (%i: %i): %s", line_num, postion, msg);
    return NULL;
}

/* 
    Expression parsing
*/

// expr -> primary_expr bin_expr
static expr_ast *parse_expr()
{
    expr_ast *lhs = parse_primary();

    // failed to parse lhs expression
    if (lhs == NULL)
        return NULL;

    return parse_bin_op(0, lhs);
}

//primary_expr -> identifier_expr | number_expr | parenthesis_expr
static expr_ast *parse_primary()
{
    switch (token_type)
    {
    case IDENT_TOK:
        return parse_ident_expr();

    case CONST_TOK:
        return parse_number();

    case LPAR_TOK:
        return parse_paren_expr();

    default:
        return log_parse_error("expected expression");
    }
}

// identifier_expr -> identifier | identifier '(' expr* ')'
static expr_ast *parse_ident_expr()
{
    expr_ast *exp;
    char identifier[IDENTIFIER_MAX_LENGTH];

    strcpy(identifier, token_id); // save current identifier
    get_next_token();             // eat identifier

    if (token_type != LPAR_TOK) // Simple variable usage
        return create_var_expr(identifier);

    get_next_token(); // Eat '('

    // default values for function calls without params
    expr_ast *params = NULL;
    int nbr_params = 0;

    // check if params exists
    if (token_type != RPAR_TOK)
    {
        expr_ast *tmp_params[MAX_ARGS]; // temporary array to store the scanned args, will be copied if parsing of param list is successful

        while (1)
        {
            if ((exp = parse_expr()) != NULL)
            {
                // push param to temporay array
                if (nbr_params >= MAX_ARGS)
                {
                    return log_parse_error("To many arguments in grammar list (max " xstr(MAX_ARGS) ")");
                }
                tmp_params[nbr_params++] = exp;
            }
            else
            {
                // failed to find expr, params array will cleanup itself
                return NULL;
            }

            if (token_type == RPAR_TOK)
            {
                // Done, create permanent copy of temp array
                params = calloc(sizeof(expr_ast), nbr_params);
                memcpy(params, tmp_params, nbr_params * sizeof(expr_ast));
                break;
            }

            if (token_type != COMM_TOK)
                return log_parse_error("expected ')' or ',' in argument list");
            get_next_token();
        }
    }
    get_next_token();

    return create_call_expr(identifier, params, nbr_params);
}

// number_expr -> NUMBER
static expr_ast *parse_number()
{
    expr_ast *e = create_const_expr(token_num);
    get_next_token();
    return e;
}

// parenthesis_expr -> '(' expr ')'
static expr_ast *parse_paren_expr()
{
    get_next_token(); // eat '('
    expr_ast *e = parse_expr();

    // Empty parenthesis
    if (parse_expr() == NULL)
        return NULL;

    if (token_type != LPAR_TOK)
        return log_parse_error("expected ')'");

    get_next_token(); // eat ')'
    return e;
}

/** 
 * @brief Helper function to determine precedence of the current token
 * @return precedence value 10, 20, 30 etc., or -1 if token does not have a precedence
*/
static int get_tok_precedence()
{
    switch (token_type)
    {
    case LESS_TOK:
        return 10;
    case ADD_TOK:
        return 20;
    case SUB_TOK:
        return 30;
    case MUL_TOK:
        return 40;

    default:
        return -1; // used to make sure anything other the binary operators have a lower precedence than parser_expr(),
                   // prevents us from eating expressions infinitely
    }
}

//bin_expr -> (bin_op primary_expr)*
static expr_ast *parse_bin_op(int expr_prec, expr_ast *lhs)
{
    // If this is a binop, find its precedence.
    while (1)
    {
        int tok_prec = get_tok_precedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        if (tok_prec < expr_prec)
            return lhs;

        int bin_op = token_type;
        get_next_token(); // eat binop

        // Try to parse rhs expression
        expr_ast *rhs = parse_primary();
        if (rhs == NULL)
            return NULL;

        int next_prec = get_tok_precedence();

        // Checks if next binary operator binds harder
        if (tok_prec < next_prec)
        {
            // if that is the case, parse that expression first
            rhs = parse_bin_op(tok_prec + 1, rhs); // next_prec will become tok_prec
            if (rhs == NULL)
                return NULL;
        }

        // Merges lhs and rhs into one expression, by updating lhs left recursion is applied
        lhs = create_bin_expr(bin_op, lhs, rhs);
    }

    get_next_token(); // eat operator
}

int main()
{
    get_next_token();
    parse_primary();
    printf("Program successfully parsed\n");
    return 0;
}