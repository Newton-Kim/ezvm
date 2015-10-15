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
    char c_value;
    int i_value;
    double f_value;
}

%start program

%%
program : entry procs { cout << "pass!" << endl;};

entry : ENTRY SYMBOL NEWLINE;

procs : proc
	| procs proc;

proc : PROC SYMBOL '(' INTEGER ')' ',' INTEGER ':' NEWLINE codes;

codes : line NEWLINE
	| codes line NEWLINE;

line : call;

call : CALL STRING '(' args ')' returns;

returns : | ',' vars;

args : | vars;
vars : var | vars ',' var;

var : STRING | SYMBOL | INTEGER;
%%
void yyerror (char const *s) {
	extern int yylineno;
	cout << "error (" << yylineno << ") " << s << endl;
}

