%{
#include "ezvm/ezvm.h"
#include "ezvm/ezlog.h"
#include <iostream>
#include <stack>
#define YYDEBUG 1
extern "C" {
	int yylex();
	void yyerror (char const *s);
}
using namespace std;
static ezVM s_vm;
ezAsmProcedure* s_proc_current = NULL;
%}

%token PROC ENTRY CALL LD MV SYMBOL STRING NEWLINE INTEGER ADDRESS

%type <s_value> PROC ENTRY CALL LD MV SYMBOL STRING NEWLINE
%type <i_value> INTEGER
%type <a_value> ADDRESS

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
program : entry procs { ezLog::logger().print("pass!"); };

entry : ENTRY SYMBOL NEWLINE { s_vm.assembler().entry($2); };

procs : proc
	| proc procs;

proc : PROC SYMBOL '(' INTEGER ')' INTEGER ':' {s_proc_current = s_vm.assembler().new_proc($2, $4, $6);}
		NEWLINE codes {s_proc_current = NULL;};

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

extern FILE * yyin;
void ezparse(FILE* fd, const string target) {
	yyin = fd;
	yyparse();
	yyin = NULL;
	if(target.empty()) s_vm.run();
	else s_vm.archive().save(target);
}

void yyerror (char const *s) {
	extern int yylineno;
	cout << "error (" << yylineno << "): " << s << endl;
}

