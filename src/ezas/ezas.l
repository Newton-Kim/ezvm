%{
#include "ezas.tab.cc.h"
%}

%option yylineno

comment #.*
integer [0-9]+
string \".*\"
symbol [a-zA-Z_][a-zA-Z_0-9]*
newline [\n\r]
%%

"call" return CALL;
".entry" return ENTRY;
".proc" return PROC;
{string} {yylval.s_value = strndup(yytext + 1, strlen(yytext) - 2); return STRING;}
{integer} {yylval.i_value = atoi(yytext); return INTEGER; }
{symbol} {yylval.s_value = strdup(yytext); return SYMBOL; }
{newline} return NEWLINE;
"("|")"|":"|"," { yylval.c_value = yytext[0]; return yytext[0];}
%%
