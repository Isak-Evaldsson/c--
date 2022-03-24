#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include "lexer.h"
#include "util.h"

// Stores the current position in the read file

// Global variable containing the state of the current token
int line_num = 1;
int postion = 0;
int token_type = -1;
int token_num = 0;
char token_id[IDENTIFIER_MAX_LENGTH];

// Internal variable storing current look-ahead char in stdin
static int current_char = ' ';

// Internal variable storing the file to be read
static FILE *src_file = NULL;

// Consumes the currnet look-ahead char, returns the old look-ahead char
static int consume()
{
    int old_char = current_char;
    current_char = fgetc(src_file);
    postion++;

    if (old_char == '\n')
    {
        line_num++;
        postion = 0;
    }
    return old_char;
}

// Reads chars from stdin with a look-ahead of 1, returns a token.
// If non suitable is found, an error is thrown.
void get_next_token()
{
    // Skips whitespace and increment line numbers
    while (isspace(current_char))
        consume();

    // Handles one char tokens
    switch (current_char)
    {
    case '{':
        consume();
        token_type = LCUR_TOK;
        return;

    case '}':
        consume();
        token_type = RCUR_TOK;
        return;

    case '(':
        consume();
        token_type = LPAR_TOK;
        return;

    case ')':
        consume();
        token_type = RPAR_TOK;
        return;

    case ';':
        consume();
        token_type = SEMI_TOK;
        return;
    case '=':
        consume();
        token_type = ASSIGN_TOK;
        return;

    case ',':
        consume();
        token_type = COMM_TOK;
        return;

    case '*':
        consume();
        token_type = MUL_TOK;
        return;

    case '+':
        consume();
        token_type = ADD_TOK;
        return;

    case '-':
        consume();
        token_type = SUB_TOK;
        return;

    case '<':
        consume();
        token_type = LESS_TOK;
        return;
    }

    // Finds identifiers and keywords, corresponds to the
    // states [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(current_char))
    {
        int i;
        char tmp[IDENTIFIER_MAX_LENGTH];
        tmp[0] = consume();

        for (i = 1; isalnum(current_char); i++)
        {
            // Ensures no buffer overflow by checking maxlength
            if (i >= IDENTIFIER_MAX_LENGTH)
                error("Lexer error", "found to long identifier '%s' at postion %i in line %i", tmp, postion, line_num);

            tmp[i] = consume();
        }
        tmp[i] = '\0';

        // Match tokens
        if (strcmp("int", tmp) == 0)
        {
            token_type = INT_TOK;
            return;
        }
        else if (strcmp("var", tmp) == 0)
        {
            token_type = VAR_TOK;
            return;
        }

        token_type = IDENT_TOK;
        strcpy(token_id, tmp);
        return;
    }

    // Finds integers, corresponds to the
    // states [0-9]+
    if (isdigit(current_char))
    {
        int num = consume() - '0';

        while (isdigit(current_char))
            num = 10 * num + (consume() - '0');

        token_type = CONST_TOK;
        token_num = num;
        return;
    }

    // Skips comments
    if (current_char == '#')
    {
        int c = consume();

        while (c != EOF && c != '\n' && c != '\r')
        {
            c = consume();
        }

        if (c != EOF)
        {
            get_next_token();
            return;
        }
    }

    // Finds EOF
    if (current_char == EOF)
    {
        token_type = EOF_TOK;
        return;
    }

    // Error if input couldn't be matched to a token
    error("Lexer error", "invalid character '%c' at postion %i in line %i", current_char, postion, line_num);
}

void init_lexer(FILE *src)
{
    // Reseting external state
    line_num = 1;
    postion = 0;
    token_type = -1;
    token_num = 0;

    // Reseting internal state
    src_file = src;
    current_char = ' ';
}