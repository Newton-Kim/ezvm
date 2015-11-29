%{
#include "ezvm/ezvm.h"
#include "ezvm/ezlog.h"
#include <iostream>
#include <map>
#include <vector>
#define YYDEBUG 1

int yylex();
void yyerror (char const *s);

using namespace std;
static ezVM s_vm;
static ezAsmProcedure* s_proc_current = NULL;
static map<string, unsigned int> s_global;
static vector<ezAddress> s_args_addr;
static vector<ezAddress> s_args_var;
%}

%token PROC ENTRY IMPORT CALL LD MV ADD SYMBOL STRING NEWLINE INTEGER ADDRESS SYMBOLIC_ADDRESS MEMORIES

%type <s_value> PROC ENTRY CALL LD MV SYMBOL STRING NEWLINE
%type <i_value> INTEGER proc_meta
%type <a_value> ADDRESS fname var
%type <sa_value> SYMBOLIC_ADDRESS

%union {
    char* s_value;
    char c_value;
    int i_value;
    double f_value;
    struct {
        size_t segment;
        size_t offset;
    } a_value;
    struct {
        char* segment;
        char* offset;
    } sa_value;
};

%start program

%%
program : import entry procs {if(s_proc_current) delete s_proc_current; ezLog::instance().debug("pass!"); };

import : | IMPORT SYMBOL NEWLINE {
		//TODO:load a script ($2+'.ezas')
		s_vm.assembler().import($2);
	};

entry : ENTRY SYMBOL NEWLINE { s_vm.assembler().entry($2); };

procs : proc
	| proc procs;

proc : PROC SYMBOL '(' INTEGER ')' INTEGER ':' NEWLINE proc_meta {if(s_proc_current) delete s_proc_current; s_proc_current = s_vm.assembler().new_proc($2, $4, $6, $9);}
		codes {s_proc_current = NULL;};

proc_meta : {$$ = 0;} | MEMORIES INTEGER NEWLINE {$$ = $2;};

codes : line NEWLINE | codes line NEWLINE;

line : | call
	| mv
	| ld
	| add;

mv : MV addrs ',' vars {s_proc_current->mv(s_args_addr, s_args_var); s_args_addr.clear(); s_args_var.clear();};

ld : LD ADDRESS ',' var var {s_proc_current->ld(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));};

add : ADD var ',' vars {s_proc_current->add(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| ADD var var ',' vars {s_proc_current->add(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	
call : CALL fname '(' vars ')' addrs {s_proc_current->call(ezAddress($2.segment, $2.offset), s_args_var, s_args_addr); s_args_addr.clear(); s_args_var.clear();};

fname : SYMBOL {$$.segment = EZ_ASM_SEGMENT_GLOBAL; $$.offset = s_vm.assembler().global($1);}
	| ADDRESS {$$ = $1;}
	| SYMBOLIC_ADDRESS {$$.segment = s_vm.assembler().segment($1.segment); $$.offset = s_vm.assembler().offset($1.segment, $1.offset);};

addrs : | addrs ADDRESS {s_args_addr.push_back(ezAddress($2.segment, $2.offset));};

vars : | vars var {s_args_var.push_back(ezAddress($2.segment, $2.offset));};

var : STRING {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant($1);}
	| SYMBOL {$$.segment = EZ_ASM_SEGMENT_GLOBAL; $$.offset = s_vm.assembler().global($1);}
	| INTEGER {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant($1);}
	| ADDRESS {$$ = $1;};
%%

extern FILE * yyin;
void ezparse(FILE* fd, const string target, const string dump) {
	yyin = fd;
	yyparse();
	yyin = NULL;
	if(!dump.empty()) 
		s_vm.dump().dump(dump);
	if(target.empty()) s_vm.run();
	else s_vm.archive().save(target);
}

void yyerror (char const *s) {
	extern int yylineno;
	cout << "error (" << yylineno << "): " << s << endl;
}

