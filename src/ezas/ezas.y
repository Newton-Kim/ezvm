%{
extern "C" {
	int yylex();
	void yyerror (char const *s);
}
#include <iostream>
using namespace std;
%}

%token PROC ENTRY CALL SYMBOL STRING NEWLINE INTEGER

%%
program : entry procs { cout << "pass!" << endl;};

entry : ENTRY':' SYMBOL NEWLINE { cout << "entry!" << endl;};

procs : proc | procs proc;

proc : PROC STRING '(' INTEGER ')' ',' INTEGER ':' NEWLINE codes;

codes : NEWLINE
	| codes line;

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

