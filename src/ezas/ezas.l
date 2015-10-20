%{
#include "ezas.tab.cc.h"
%}

%option yylineno

comment #.*
integer [0-9]+
string \".*\"
symbol [a-zA-Z_][a-zA-Z_0-9]*
newline [\n\r]
address [cglp]:[0-9]+
symbolic_address [a-zA-Z_][a-zA-Z_0-9]*:[a-zA-Z_][a-zA-Z_0-9]*
sp [ \t]
%%

"call" return CALL;
"ld" return LD;
"mv" return MV;
".import" return IMPORT;
".entry" return ENTRY;
".proc" return PROC;
{string} {yylval.s_value = strndup(yytext + 1, strlen(yytext) - 2); return STRING;}
{integer} {yylval.i_value = atoi(yytext); return INTEGER; }
{symbol} {yylval.s_value = strdup(yytext); return SYMBOL; }
{address} {
	yylval.a_value.segment = yytext[0];
	yylval.a_value.offset = atoi(yytext + 2);
	return ADDRESS;
}
{symbolic_address} {
	yylval.sa_value.segment = strtok(yytext, ":");
	yylval.sa_value.offset = strtok(NULL, ":");
	return SYMBOLIC_ADDRESS;
}
{newline} return NEWLINE;
"("|")"|":"|"," { yylval.c_value = yytext[0]; return yytext[0];}
{sp} {}
%%
