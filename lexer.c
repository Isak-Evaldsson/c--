#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>

#define IDENTIFIER_MAX_LENGTH 40

/* Defies all possible tokens */
typedef enum
{
    L_PAREN,     // '('
    R_PAREN,     // ')'
    L_CURLY,     // '{'
    R_CURLY,     // '}'
    FUNC_DEF,    // "def"
    IDENTIFIER,  // [a-zA-Z][a-zA-Z0-9]*
    INTEGER,     // [0-9]+
    END_OF_FILE, // EOF
} token_type;

/* Structs */
typedef struct token_t token_t;

// Struct conting a token of a certain type, some types (integer and identifier) stores data
struct token_t
{
    token_type type; // What kind of token it is
    int line_num;    // Which line source line the token was found on
    union            // data within the token, null is not used
    {
        char *string; // Stores indentifiers
        int number;   // Stores integers
    } data;
};

// Stores the current position in the read file
static int line_num = 1;
static int postion = 0;

// utility function for error messages
void error(const char *fmt, ...)
{
    va_list ap;
    char buf[BUFSIZ];

    // Creates formated string in buf
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);

    // Prints buffer and exits with error
    fprintf(stderr, "Lexer error: %s\n", buf);
    exit(1);
}

/* 
    Token creation functions
*/
token_t *create_token_w_data(token_type type, int number, char *string)
{
    token_t *t = calloc(sizeof(token_t), 1);
    t->line_num = line_num;
    t->type = type;

    switch (type)
    {
    case IDENTIFIER:
        // Allocate and copy string
        t->data.string = calloc(sizeof(char), strlen(string) + 1);
        strcpy(t->data.string, string);
        break;
    case INTEGER:
        t->data.number = number;
        break;
    }

    return t;
}

token_t *create_token(token_type type)
{
    return create_token_w_data(type, 0, NULL);
}

void free_token(token_t *t)
{
    if (t->type == IDENTIFIER)
    {
        char *s = t->data.string;
        free(s);
    }
    free(t);
}

// Lets tokenizer look-ahead one char
static int current_char = ' ';

// Consumes the currnet look-ahead char, returns the old look-ahead char
static int consume()
{
    int old_char = current_char;
    current_char = getchar();
    postion++;

    if (current_char == '\n')
    {
        line_num++;
        postion = 0;
    }
    return old_char;
}

// Reads chars from stdin with a look-ahead of 1, returns a token.
// If non suitable is found, an error is thrown.
token_t *get_token()
{
    // Skips whitespace and increment line numbers
    while (isspace(current_char))
        consume();

    // Handles one char tokens
    switch (current_char)
    {
    case '{':
        consume();
        return create_token(L_CURLY);

    case '}':
        consume();
        return create_token(R_CURLY);

    case '(':
        consume();
        return create_token(L_PAREN);

    case ')':
        consume();
        return create_token(R_PAREN);
    }

    // Finds identifiers and keywords, corresponds to the
    // states [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(current_char))
    {
        int i;
        char tmp[IDENTIFIER_MAX_LENGTH + 1];
        tmp[0] = consume();

        for (i = 1; isalnum(current_char); i++)
        {
            // Ensures no buffer overflow by checking maxlength
            if (i >= IDENTIFIER_MAX_LENGTH)
                error("found to long identifier '%s' at postion %i in line %i", tmp, postion, line_num);

            tmp[i] = consume();
        }
        tmp[i] = '\0';

        // Match tokens
        if (strcmp("def", tmp) == 0)
        {
            return create_token(FUNC_DEF);
        }

        return create_token_w_data(IDENTIFIER, 0, tmp);
    }

    // Finds integers, corresponds to the
    // states [0-9]+
    if (isdigit(current_char))
    {
        int num = consume() - '0';

        while (isdigit(current_char))
            num = 10 * num + (consume() - '0');

        printf("NUMBER BOOM\n");
        return create_token_w_data(INTEGER, num, NULL);
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
            return get_token();
    }

    // Finds EOF
    if (current_char == EOF)
        return create_token(END_OF_FILE);

    // Error if input couldn't be matched to a token
    error("invalid character '%c' at postion %i in line %i", current_char, postion, line_num);
    return NULL;
}

int main()
{
    token_t *t;
    token_type type;
    do
    {
        t = get_token();
        type = t->type;
        printf("Found token of type %d, at line %d \n", type, t->line_num);
        free_token(t);
    } while (type != END_OF_FILE);
    return 0;
}