%{
extern "C" {
	int yylex();
	void yyerror (char const *s);
}
#include <iostream>
#define YYDEBUG 1
using namespace std;
%}

%token PROC ENTRY CALL SYMBOL STRING NEWLINE INTEGER

%union {
    char* s_value;
    int i_value;
    double f_value;
}

%start program

%%
program : entry procs { cout << "pass!" << endl;};

entry : ENTRY SYMBOL NEWLINE;

procs : proc  { cout << "pass" << endl; }| procs proc;

proc : PROC STRING '(' INTEGER ')' ',' INTEGER ':' NEWLINE codes;

codes : 
	| codes line NEWLINE;

line : call;
call : CALL STRING '(' vars ')'
	| CALL STRING '(' vars ')' ',' vars;

vars :
	| var
	| var ',' vars;

var : STRING | SYMBOL | INTEGER;
%%
void yyerror (char const *s) {
	extern int yylineno;
	cout << "error (" << yylineno << ") " << s << endl;
}

