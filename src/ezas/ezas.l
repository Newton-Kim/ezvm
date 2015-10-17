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
".entry" return ENTRY;
".proc" return PROC;
{string} {yylval.s_value = strndup(yytext + 1, strlen(yytext) - 2); return STRING;}
{integer} {yylval.i_value = atoi(yytext); return INTEGER; }
{symbol} {yylval.s_value = strdup(yytext); return SYMBOL; }
{address} {
	char* str = strtok(yytext, ":");
	yylval.a_value.segment = strdup(str);
	str = strtok(NULL, ":");
	yylval.a_value.numeric_offset = atoi(str);
	yylval.a_value.symbolic_offset= NULL;
	return ADDRESS;
}
{symbolic_address} {
	char* str = strtok(yytext, ":");
	yylval.a_value.segment = strdup(str);
	str = strtok(NULL, ":");
	yylval.a_value.symbolic_offset= strdup(str);
	return ADDRESS;
}

{newline} return NEWLINE;
"("|")"|":"|"," { yylval.c_value = yytext[0]; return yytext[0];}
{sp} {}
%%
