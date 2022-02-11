#include "lexer.h"
#include "parser.h"

int main()
{
    // set the lexer to read from stdin
    src_file = stdin;

    if (parse())
    {
        printf("Program successfully parsed\n");
    }
    else
    {
        printf("Parsing error, aborting compilation\n");
    }
    return 0;
}