#include "lexer.h"
#include "parser.h"

int main()
{
    init_lexer(stdin);

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