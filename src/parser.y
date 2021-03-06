%{
#include <stdio.h>
#include "ast.h"
#include "util.h"
#include "errormsg.h"

// Lexing function generated by flex
int yylex(void);

// Root node for ast 
AST_func_list *ast_root;

// Bison error handling function
void yyerror(char *s)
{
    EM_error(EM_tokPos, "%s", s);
}
%}

%union {
        // Terminal types
        int pos;
        int ival;
        char* sval;

        // Non-terminal types
        AST_func_list *funcs;
        AST_func_def *func_def;
        AST_prototype *proto;
        AST_param_list *param;
        AST_stmt_list *stmts;
        AST_stmt *stmt;
        AST_expr *expr;
       }

%token <sval> ID
%token <ival> LITERAL
%token  INT
        LPAREN 
        RPAREN
        SEMI
        COMMA
        LCURLY
        RCURLY
        EQ       

%type <proto> prototype
%type <param> param_list param_list2
%type <sval>  param
%type <funcs> func_list
%type <func_def> func_defintion
%type <stmts> stmt_list
%type <stmt> statement
%type <expr> expression

%start program

%%

program: func_list { ast_root = $1; }

func_list:
    func_defintion              { $$ = create_func_list($1, NULL); }
    | func_defintion func_list  { $$ = create_func_list($1, $2); }

func_defintion: prototype LCURLY stmt_list RCURLY { $$ = create_func_def($1, $3); }

prototype: INT ID LPAREN param_list RPAREN { $$ = create_prototype_ast($2, $4); }

param_list:
    /* epsiolon */              { $$ = NULL; }
    | param                     { $$ = create_param_list($1, NULL); }
    | param COMMA param_list2   { $$ = create_param_list($1, $3); }

param_list2:        
    param                       { $$ = create_param_list($1, NULL); }
    | param COMMA param_list2   { $$ = create_param_list($1, $3); }

param: INT ID           { $$ = $2; }

stmt_list:
    /* epsilon */               { $$ = NULL; }
    | statement stmt_list    { $$ = create_stmt_list($1, $2); }

statement: 
    INT ID SEMI                 { $$ = create_stmt(STMT_VAR_DECL, $2, NULL); }
    | INT ID EQ expression SEMI { $$ = create_stmt(STMT_VAR_DECL, $2, $4); }


expression: LITERAL { $$ = create_expr(EXPR_LITERAL, $1); }