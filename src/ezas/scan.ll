%{
/* Copyright (C) 2015 Newton Kim
* 
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditiong:
* 
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
*/
#include "ezvm/ezaddr.h"
#include "parse.hh"
%}

%option yylineno

comment #.*
integer [+-]?[0-9]+
string \"[^"]*\"
symbol _[a-zA-Z_][a-zA-Z_0-9]*
label  :.*
address [cgrp][0-9]+
newline [\n\r]
sp [ \t]
%%

"true" {yylval.b_value = true; return BOOLEAN;}
"false" {yylval.b_value = false; return BOOLEAN;}
"add" return ADD;
"and" return AND;
"beq" return BEQ;
"bge" return BGE;
"blt" return BLT;
"bne" return BNE;
"bra" return BRA;
"call" return CALL;
"cmp" return CMP;
"div" return DIV;
"ld" return LD;
"lsl" return LSL;
"lsr" return LSR;
"mod" return MOD;
"mul" return MUL;
"mv" return MV;
"neg" return NEG;
"not" return NOT;
"or" return OR;
"ret" return RET;
"sub" return SUB;
"xor" return XOR;
".import" return IMPORT;
".entry" return ENTRY;
".proc" return PROC;
".scope" return SCOPE;
".scpkey" return SCOPE_KEY;
".memories" return MEMORIES;
"stdout" { yylval.i_value = 1; return INTEGER; }
"stderr" { yylval.i_value = 2; return INTEGER; }
{string} {yylval.s_value = strndup(yytext + 1, strlen(yytext) - 2); return STRING;}
{integer} {yylval.i_value = atoi(yytext); return INTEGER; }
{symbol} {yylval.s_value = strdup(yytext + 1); return SYMBOL; }
{label} {yylval.s_value = strdup(yytext + 1); return LABEL; }
{address} {
	switch(yytext[0]){
		case 'g':
			yylval.a_value.segment = EZ_ASM_SEGMENT_GLOBAL;
			break;
		case 'c':
			yylval.a_value.segment = EZ_ASM_SEGMENT_CONSTANT;
			break;
		case 'r':
			yylval.a_value.segment = EZ_ASM_SEGMENT_LOCAL;
			break;
		case 'p':
			yylval.a_value.segment = EZ_ASM_SEGMENT_PARENT;
			break;
		default:
			fprintf(stderr, "error (%d): invalid segment %c\n", yylineno, yytext[0]);
			break;
	}
	yylval.a_value.offset = atoi(yytext + 1);
	return ADDRESS;
}
{newline} return NEWLINE;
"("|")"|":"|"," { yylval.c_value = yytext[0]; return yytext[0];}
{sp} {}
%%
