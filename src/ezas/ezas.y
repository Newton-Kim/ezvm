%{
extern "C" {
	int yylex();
	void yyerror (char const *s);
}
#include <iostream>
#define YYDEBUG 1
using namespace std;
%}

%token PROC ENTRY CALL LD MV SYMBOL STRING NEWLINE INTEGER ADDRESS

%union {
    char* s_value;
    char c_value;
    int i_value;
    double f_value;
    struct {
        char* segment;
        char* symbolic_offset;
        unsigned int numeric_offset;
    } a_value;
};

%start program

%%
program : entry procs { cout << "pass!" << endl;};

entry : ENTRY SYMBOL NEWLINE;

procs : proc
	| proc procs;

proc : PROC SYMBOL '(' INTEGER ')' INTEGER ':' NEWLINE codes;

codes : line NEWLINE | codes line NEWLINE;

line : | call
	| mv
	| ld;

mv : MV ADDRESS var;

ld : LD ADDRESS var var;

call : CALL fname '(' vars ')' addrs;

fname : SYMBOL | ADDRESS;

addrs : | addrs ADDRESS;

vars : | vars var;

var : STRING | SYMBOL | INTEGER | ADDRESS;
%%
void yyerror (char const *s) {
	extern int yylineno;
	cout << "error (" << yylineno << "): " << s << endl;
}

