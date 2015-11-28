%{
#include "ezvm/ezaddr.h"
#include "ezas.tab.cc.h"
%}

%option yylineno

comment #.*
integer [0-9]+
string \"[^"]*\"
symbol [a-zA-Z_][a-zA-Z_0-9]*
newline [\n\r]
address [cglp]:[0-9]+
symbolic_address [a-zA-Z_][a-zA-Z_0-9]*:[a-zA-Z_][a-zA-Z_0-9]*
sp [ \t]
%%

"call" return CALL;
"ld" return LD;
"mv" return MV;
"add" return ADD;
".import" return IMPORT;
".entry" return ENTRY;
".proc" return PROC;
".memories" return MEMORIES;
"stdout" { yylval.i_value = 1; return INTEGER; }
"stderr" { yylval.i_value = 2; return INTEGER; }
{string} {yylval.s_value = strndup(yytext + 1, strlen(yytext) - 2); return STRING;}
{integer} {yylval.i_value = atoi(yytext); return INTEGER; }
{symbol} {yylval.s_value = strdup(yytext); return SYMBOL; }
{address} {
	switch(yytext[0]){
		case 'g':
			yylval.a_value.segment = EZ_ASM_SEGMENT_GLOBAL;
			break;
		case 'c':
			yylval.a_value.segment = EZ_ASM_SEGMENT_CONSTANT;
			break;
		case 'l':
			yylval.a_value.segment = EZ_ASM_SEGMENT_LOCAL;
			break;
		case 'p':
			yylval.a_value.segment = EZ_ASM_SEGMENT_PARENT;
			break;
		default:
			fprintf(stderr, "error (%d): invalid segment %c\n", yylineno, yytext[0]);
			break;
	}
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
