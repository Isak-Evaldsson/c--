#include "ast.h"
#include "util.h"

AST_func_list *create_func_list(AST_func_def *def, AST_func_list *next)
{
    AST_func_list *list = xmalloc(sizeof(AST_func_list));
    list->func_def = def;
    list->next = next;
    return list;
}

AST_func_def *create_func_def(AST_prototype *proto, AST_stmt_list *stmts)
{
    AST_func_def *func_decl = xmalloc(sizeof(AST_func_def));
    func_decl->proto = proto;
    func_decl->stmts = stmts;
    return func_decl;
}

AST_prototype *create_prototype_ast(char *identifier, AST_param_list *params)
{
    AST_prototype *proto = xmalloc(sizeof(AST_prototype));
    proto->identifier = identifier;
    proto->params = params;
    return proto;
}

AST_param_list *create_param_list(char *identifier, AST_param_list *next)
{
    AST_param_list *param = xmalloc(sizeof(AST_param_list));
    param->identifier = identifier;
    param->next = next;
    return param;
}

void print_proto(AST_prototype *proto, FILE *fp)
{
    AST_param_list *param;

    fprintf(fp, "Prototype: %s\n", proto->identifier);

    for (param = proto->params; param != NULL; param = param->next) {
        for (size_t i = 0; i < INDENT; i++)
            fputc(' ', fp);

        fprintf(fp, "Param: %s\n", param->identifier);
    }
}

void print_func_list(AST_func_list *list, FILE *fp)
{
    print_proto(list->func_def->proto, fp);
    if (list->next)
        print_func_list(list->next, fp);
}