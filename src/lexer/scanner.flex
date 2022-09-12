%{
#include <string.h>
#include "util.h"
#include "ast.h"
#include "parser.tab.h"
#include "errormsg.h"

int charPos=1;

int yywrap(void)
{
    charPos=1;
    return 1;
}


void adjust(void)
{
    EM_tokPos=charPos;
    charPos+=yyleng;
}

%}

 // Regex macros simplifying translations
ID  [a-zA][a-zA-Z0-9_]*
INT [1-9][0-9]*|[0-9]

%%
"//".*      { /* Ignore comment */ }
" "	        { adjust(); continue; }
\n	        { adjust(); EM_newline(); continue; }
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
.	        { adjust(); EM_error(EM_tokPos,"illegal token '%s'", yytext); yyterminate(); }
%%