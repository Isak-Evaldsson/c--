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
void yyerror(const char *s)
{
    EM_error(EM_tokPos, "%s", s);
}
%}

%define parse.error verbose

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
        AST_expr_list *exprs;
        AST_expr *expr;
        AST_func_call *call;
       }

%token <sval> ID
%token <ival> INTEGER
%token  INT
        LPAREN 
        RPAREN
        SEMI
        COMMA
        LCURLY
        RCURLY
        EQ
        PLUS
        MINUS
        MUL
        DIV
        RETURN
        WHILE

%type <proto> prototype
%type <param> param_list param_list2
%type <sval>  param
%type <funcs> func_list
%type <func_def> func_defintion
%type <stmts> stmt_list
%type <stmt> statement while_stmt return_stmt
%type <exprs> arg_list arg_list2
%type <expr> expression term factor
%type <call> func_call

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
    | ID EQ expression SEMI     { $$ = create_stmt(STMT_ASSIGN, $1, $3); }
    | func_call SEMI            { $$ = create_func_call_stmt($1); }
    | return_stmt
    | while_stmt
    
return_stmt: RETURN expression SEMI { $$ = create_stmt(STMT_RETURN, NULL, $2); }

while_stmt:
    WHILE LPAREN expression RPAREN LCURLY stmt_list RCURLY { $$ = create_while_stmt($3, $6); }
    | WHILE LPAREN expression RPAREN statement             { $$ = create_while_stmt($3, create_stmt_list($5, NULL)); }

func_call: ID LPAREN arg_list RPAREN { $$ = create_func_call($1, $3); }

arg_list:
    /* epsiolon */               { $$ = NULL; }
    | expression                 { $$ = create_expr_list($1, NULL); }
    | expression COMMA arg_list2 { $$ = create_expr_list($1, $3); }

arg_list2:        
    expression                   { $$ = create_expr_list($1, NULL); }
    | expression COMMA arg_list2 { $$ = create_expr_list($1, $3); }

expression:
    term
    | expression PLUS term  { $$ = create_binary_expr(BINOP_PLUS, $1, $3); }
    | expression MINUS term { $$ = create_binary_expr(BINOP_MINUS, $1, $3); }

term:
    factor
    | term MUL factor { $$ = create_binary_expr(BINOP_MUL, $1, $3); }
    | term DIV factor { $$ = create_binary_expr(BINOP_DIV, $1, $3); }

factor: 
    LPAREN expression RPAREN { $$ = $2; }
    | INTEGER                { $$ = create_literal_expr(EXPR_LITERAL, $1); }
    | ID                     { $$ = create_var_use_expr($1); }
    | MINUS factor           { $$ = create_neg_expr($2); }
    | func_call              { $$ = create_func_call_expr($1); }