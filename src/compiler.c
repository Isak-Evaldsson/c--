#include <stdio.h>

#include "ast.h"
#include "errormsg.h"
#include "parser.h"
#include "util.h"

int main(int argc, char** argv)
{
    char* file_name;
    AST_prototype* proto;

    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    file_name = argv[1];

    if (!(proto = parse(file_name))) {
        exit(1);
    }

    printf("Compilation successful\n");
    print_proto(proto, stdout);
    return 0;
}