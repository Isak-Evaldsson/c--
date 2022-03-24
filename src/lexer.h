#ifndef LEXER_HEADER
#define LEXER_HEADER
#include <stdio.h>

// Defines number of spaces for each level of indent
#define INDENT 4

// Defines max length of identifiers in order to prevent buffer overflows
#define IDENTIFIER_MAX_LENGTH 40

/* Defines all possible tokens, post _TOK is used to avoid enum ambiguities */
typedef enum
{
    // Formating chars
    LPAR_TOK, // '('
    RPAR_TOK, // ')'
    LCUR_TOK, // '{'
    RCUR_TOK, // '}'
    SEMI_TOK, // ';'
    COMM_TOK, // ','

    // Operators
    ASSIGN_TOK, // '='
    ADD_TOK,    // '+'
    SUB_TOK,    // '-'
    LESS_TOK,   // '<'
    MUL_TOK,    // '*'

    // Keywords
    INT_TOK, // "int"
    VAR_TOK, // "var"

    // Others
    IDENT_TOK, // identifier: [a-zA-Z][a-zA-Z0-9]*
    CONST_TOK, // integer: [0-9]+
    EOF_TOK,   // EOF
} tokens;

// Global variables storing the state of the current token
extern int line_num;                         // current line in file
extern int postion;                          // current line position
extern int token_type;                       // type of token, see tokens enum
extern int token_num;                        // stores number for const token
extern char token_id[IDENTIFIER_MAX_LENGTH]; // store identifier for id token

// Reads chars from stdin with a look-ahead of 1 and updates the state
// of the variables defined above. If non suitable is found, an error is thrown.
void get_next_token();

// Initialises the lexer by setting the correctenteral state,
// including the file to be read
void init_lexer(FILE *src);

#endif