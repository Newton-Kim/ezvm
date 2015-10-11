%{
#include <iostream>
%}

%token PROC ENTRY CALL SYMBOL STRING NEWLINE INTEGER

%%
program : entry procs;

entry : ENTRY':' SYMBOL;

procs : proc | procs proc;

proc : PROC STRING '(' INTEGER ')' ',' INTEGER ':' NEWLINE codes

codes :
	| codes call

call : CALL STRING '(' vars ')'
	| CALL STRING '(' vars ')' ',' vars

vars :
	| var
	| var ',' vars

var : STRING | SYMBOL | INTEGER
%%
