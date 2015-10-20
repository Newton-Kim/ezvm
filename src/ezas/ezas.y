%{
#include "ezvm/ezvm.h"
#include "ezvm/ezlog.h"
#include <iostream>
#include <map>
#include <vector>
#define YYDEBUG 1
extern "C" {
	int yylex();
	void yyerror (char const *s);
}
using namespace std;
static ezVM s_vm;
static ezAsmProcedure* s_proc_current = NULL;
static unsigned int s_global_index = 4;
static map<string, unsigned int> s_global;
static vector<ezAddress> s_args1;
static vector<ezAddress> s_args2;
%}

%token PROC ENTRY IMPORT CALL LD MV SYMBOL STRING NEWLINE INTEGER ADDRESS SYMBOLIC_ADDRESS

%type <s_value> PROC ENTRY CALL LD MV SYMBOL STRING NEWLINE
%type <i_value> INTEGER
%type <a_value> ADDRESS fname
%type <sa_value> SYMBOLIC_ADDRESS

%union {
    char* s_value;
    char c_value;
    int i_value;
    double f_value;
    struct {
        char segment;
        unsigned int offset;
    } a_value;
    struct {
        char* segment;
        char* offset;
    } sa_value;
};

%start program

%%
program : import entry procs { ezLog::logger().print("pass!"); };

import : | IMPORT SYMBOL NEWLINE { s_vm.assembler().import($2); };

entry : ENTRY SYMBOL NEWLINE { s_vm.assembler().entry($2); };

procs : proc
	| proc procs;

proc : PROC SYMBOL '(' INTEGER ')' INTEGER ':' {s_proc_current = s_vm.assembler().new_proc($2, $4, $6);}
		NEWLINE codes {s_proc_current = NULL;};

codes : line NEWLINE | codes line NEWLINE;

line : | call
	| mv
	| ld;

mv : MV mvaddrs ',' mvvars;

mvaddrs : ADDRESS | mvaddrs ADDRESS;

mvvars : var | mvvars var;

ld : LD ADDRESS ',' var var;

call : CALL fname '(' vars ')' addrs;

fname : SYMBOL {$$.segment = 'g'; $$.offset = s_vm.assembler().offset(EZ_ASM_SEGMENT_GLOBAL, $1);}
	| ADDRESS {$$ = $1;}
	| SYMBOLIC_ADDRESS {$$.segment = s_vm.assembler().segment($1.segment);; $$.offset = s_vm.assembler().offset($1.segment, $1.offset);};

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

