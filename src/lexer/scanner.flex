%{
#include <string.h>
#include <stdio.h>
#include "util.h"
#include "ast.h"
#include "parser.tab.h"
#include "error_list.h"
#include "parser.h"

int line_num=1;
int line_pos=1;
int token_pos = 1;

int yywrap()
{
    line_pos=1;
    return 1;
}

void adjust()
{
    token_pos = line_pos + 1;
    line_pos += yyleng;
}

void newline() {
    line_num++;
    line_pos = 0;
}

void token_error() {
    error_list_add(line_num, line_pos, "illegal token '%s'", yytext); 
}

%}

 // Regex macros simplifying translations
ID  [a-zA][a-zA-Z0-9_]*
INT [1-9][0-9]*|[0-9]

%%
"//".*      { /* Ignore comment */ }
" "	        { adjust(); continue; }
\n	        { adjust(); newline(); continue; }
"("	        { adjust(); return LPAREN; }
")"	        { adjust(); return RPAREN; }
"{"	        { adjust(); return LCURLY; }
"}"	        { adjust(); return RCURLY; }
";"         { adjust(); return SEMI; }
","         { adjust(); return COMMA; }
"="         { adjust(); return EQ; }
"int"  	    { adjust(); return INT; }
"return"    { adjust(); return RETURN; }
"while"     { adjust(); return WHILE; }
"if"        { adjust(); return IF; }
"else"      { adjust(); return ELSE; }
"+"  	    { adjust(); return PLUS; }
"-"  	    { adjust(); return MINUS; }
"*"  	    { adjust(); return MUL; }
"/"  	    { adjust(); return DIV; }
{INT}       { adjust(); yylval.ival=atoi(yytext); return INTEGER; }
{ID}        { adjust(); yylval.sval=strdup(yytext); return ID; }
.	        { adjust(); token_error(); yyterminate(); }
%%