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
#include "ezvm/ezvm.h"
#include "ezvm/ezlog.h"
#include <iostream>
#include <map>
#include <vector>
#define YYDEBUG 1

/** \page ezas ezas
 * \section ezas-brief Brief
 * ezAssembler is an assembler language which is dedicated to ezVM.
 * \section ezas-doc Document
 * \subsection ezas-import .import
 * \subsubsection ezas-import-syntax Syntax
 * \subsubsection ezas-import-operation Operation
 * \subsubsection ezas-import-description Description
 * \subsubsection ezas-import-notes Notes
 * \subsubsection ezas-import-example Example
 * \subsection ezas-entry .entry
 * \subsubsection ezas-entry-syntax Syntax
 * \subsubsection ezas-entry-operation Operation
 * \subsubsection ezas-entry-description Description
 * \subsubsection ezas-entry-notes Notes
 * \subsubsection ezas-entry-example Example
 * \subsection ezas-proc .proc
 * \subsubsection ezas-proc-syntax Syntax
 * \subsubsection ezas-proc-operation Operation
 * \subsubsection ezas-proc-description Description
 * \subsubsection ezas-proc-notes Notes
 * \subsubsection ezas-proc-example Example
 * \subsection ezas-memory .memories
 * \subsubsection ezas-memory-syntax Syntax
 * \subsubsection ezas-memory-operation Operation
 * \subsubsection ezas-memory-description Description
 * \subsubsection ezas-memory-notes Notes
 * \subsubsection ezas-memory-example Example
 * \section ezas-address Address
 * \subsection ezas-address-syntax Syntax
 * [g|c|l|p|string]+:[0-9|string]+\n
 * \subsection ezas-address-operation Operation
 * \subsection ezas-address-description Description
 * Address is described by segment and offset. 'g' stands for a global segment, 'c' stands for a constant segment, 'l' stands for a local segment, and 'p' stands for a parent local segment on the scope. 'string' comes from the name of .import.
 * \subsection ezas-address-notes Notes
 * \subsection ezas-address-example Example
 * l:2\n
 * io:print\n
 * \section Instructions
 * The instructions are inspired by triVM instruction set. See 'TriVM intermediate language reference manual' by Neil Jonson.
 * \subsection ezas-add add
 * \subsubsection ezas-add-syntax Syntax
 * add rD [rC]?, [rS]+\n
 * \subsubsection ezas-add-operation Operation
 * rD = sum(rS)\n
 * rC = condition(sum(rS))\n
 * \subsubsection ezas-add-description Description
 * The add instruction adds together the contents of rSs, placing the result in rD. Optionally, the condition codes of the esult can be placed in rC.\n
 * \subsubsection ezas-add-notes Notes
 * This instructions applied to both signed and unsigned values.\n
 * \subsubsection ezas-add-example Example
 * add l:2, l:0 l:1\n
 * add l:3, 1 2 3 4
 * \subsection ezas-beq beq
 * \subsubsection ezas-beq-syntax Syntax
 * beq rS , label\n
 * \subsubsection ezas-beq-operation Operation
 * if rS == EQU AL then goto label\n
 * \subsubsection ezas-beq-description Description
 * The beq instruction branches to the given label if the condition in rS is equal.\n
 * \subsubsection ezas-beq-notes Notes
 * This instruction is applicable to both integer and floading-point comparisons.\n
 * \subsubsection ezas-beq-example Example
 * beq l:3 int_zero
 * \subsection ezas-blt blt
 * \subsubsection ezas-blt-syntax Syntax
 * blt rS, label\n
 * \subsubsection ezas-blt-operation Operation
 * if rS == LESS-THAN then goto label\n
 * \subsubsection ezas-blt-description Description
 * The blt instruction branches to the given label if the condition in rS is less-than.\n
 * \subsubsection ezas-blt-notes Notes
 * The unsigned equivalent is bbl. This instruction is also appplicable to floating point operations.
 * \subsubsection ezas-blt-example Example
 * blt l:3 int_neg
 * \subsection ezas-bra bra
 * \subsubsection ezas-bra-syntax Syntax
 * bra label\n
 * \subsubsection ezas-bra-operation Operation
 * goto label\n
 * \subsubsection ezas-bra-description Description
 * Th bra instruction branches to ghe label identified either by the given label name.\n
 * \subsubsection ezas-bra-notes Notes
 * \subsubsection ezas-bra-example Example
 * bra int_end
 * \subsection ezas-call call
 * \subsubsection ezas-call-syntax Syntax
 * call rS ( [rA]* ) [rR]*\n
 * \subsubsection ezas-call-operation Operation
 * call rS\n
 * \subsubsection ezas-call-description Description
 * The call instruction calls the procedure identified by rS. Arguments, rA, are passed to the callee. On return (see the et instruction) results are assigned to the rRs.
 * \subsubsection ezas-call-notes Notes
 * The target address pointed to by rS must be the biginning of a procedure identified by a visibile procedure identifier.\n
 * \subsubsection ezas-call-example Example
 * call io:print (stdout l:2 "=" l:0 "+" l:1)\n
 * \subsection ezas-mv mv
 * \subsubsection ezas-mv-syntax Syntax
 * mv [rD]+, [rS]+\n
 * \subsubsection ezas-mv-operation Operation
 * rD = rS\n
 * \subsubsection ezas-mv-description Description
 * The mv instruction moves the value in rS into rD.
 * \subsubsection ezas-mv-notes Notes
 * \subsubsection ezas-mv-example Example
 * mv l:0 l:1, 1 2
 * \subsection ezas-sub sub
 * \subsubsection ezas-sub-syntax Syntax
 * sub rD [rC]?, [rS]+\n
 * \subsubsection ezas-sub-operation Operation
 * rD = rS[i] - rS[i+1] in {rS}\n
 * rC = condition(rS[i] - rS[i+1])\n
 * \subsubsection ezas-sub-description Description
 * The sub instruction subtacts the contents of rSs, placing the result in rD. Optionally, the condition codes of the result can be placed in rC.
 * \subsubsection ezas-sub-notes Notes
 * \subsubsection ezas-sub-example Example
 * sub l:2, l:0 l:1\n
 * sub l:2 l:3, l:0 l:1
 */

int yylex();
void yyerror (char const *s);

using namespace std;
static ezVM s_vm;
static ezAsmProcedure* s_proc_current = NULL;
static map<string, unsigned int> s_global;
static vector<ezAddress> s_args_addr;
static vector<ezAddress> s_args_var;
%}

%token PROC ENTRY IMPORT
%token ADD AND BEQ BLT BRA CALL DIV LD MOD MUL MV NEG NOT OR SUB XOR
%token SYMBOL STRING NEWLINE INTEGER ADDRESS SYMBOLIC_ADDRESS MEMORIES LABEL BOOLEAN

%type <s_value> PROC ENTRY CALL LD MV SYMBOL STRING NEWLINE LABEL
%type <i_value> INTEGER proc_meta
%type <a_value> ADDRESS fname var
%type <sa_value> SYMBOLIC_ADDRESS
%type <b_value> BOOLEAN

%union {
    bool b_value;
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

line : | add
	| and
	| beq
	| blt
	| bra
	| call
	| div
	| ld
	| mod
	| mul
	| mv
	| neg
	| not
	| or 
	| sub 
	| xor 
	| label;

add : ADD var ',' vars {s_proc_current->add(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| ADD var var ',' vars {s_proc_current->add(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

and : AND var ',' vars {s_proc_current->bitwise_and(ezAddress($2.segment, $2.offset), s_args_var); s_args_var.clear(); s_args_var.clear();}
	| AND var var ',' vars {s_proc_current->bitwise_and(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

beq : BEQ var SYMBOL {s_proc_current->beq(ezAddress($2.segment, $2.offset), $3);}

blt : BLT var SYMBOL {s_proc_current->blt(ezAddress($2.segment, $2.offset), $3);}

bra : BRA SYMBOL {s_proc_current->bra($2);}

call : CALL fname '(' vars ')' addrs {s_proc_current->call(ezAddress($2.segment, $2.offset), s_args_var, s_args_addr); s_args_addr.clear(); s_args_var.clear();};

div : DIV var ',' vars {s_proc_current->div(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| DIV var var ',' vars {s_proc_current->div(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

ld : LD ADDRESS ',' var var {s_proc_current->ld(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));};

mod : MOD var ',' vars {s_proc_current->mod(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| MOD var var ',' vars {s_proc_current->mod(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

mul : MUL var ',' vars {s_proc_current->mul(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| MUL var var ',' vars {s_proc_current->mul(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

mv : MV addrs ',' vars {s_proc_current->mv(s_args_addr, s_args_var); s_args_addr.clear(); s_args_var.clear();};

neg : NEG var ',' var {s_proc_current->neg(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset));}
	| NEG var var ',' var {s_proc_current->neg(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset));}

not : NOT var ',' var {s_proc_current->bitwise_not(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset));}
	| NOT var var ',' var {s_proc_current->bitwise_not(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset));}

or : OR var ',' vars {s_proc_current->bitwise_or(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| OR var var ',' vars {s_proc_current->bitwise_or(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

sub : SUB var ',' vars {s_proc_current->sub(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| SUB var var ',' vars {s_proc_current->sub(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

xor : XOR var ',' vars {s_proc_current->bitwise_xor(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| XOR var var ',' vars {s_proc_current->bitwise_xor(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

label : ':' SYMBOL {s_proc_current->label($2);}

fname : SYMBOL {$$.segment = EZ_ASM_SEGMENT_GLOBAL; $$.offset = s_vm.assembler().global($1);}
	| ADDRESS {$$ = $1;}
	| SYMBOLIC_ADDRESS {$$.segment = s_vm.assembler().segment($1.segment); $$.offset = s_vm.assembler().offset($1.segment, $1.offset);};

addrs : | addrs ADDRESS {s_args_addr.push_back(ezAddress($2.segment, $2.offset));};

vars : | vars var {s_args_var.push_back(ezAddress($2.segment, $2.offset));};

var : STRING {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant($1);}
	| SYMBOL {$$.segment = EZ_ASM_SEGMENT_GLOBAL; $$.offset = s_vm.assembler().global($1);}
	| INTEGER {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant($1);}
	| BOOLEAN {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant($1);}
	| ADDRESS {$$ = $1;};
%%

extern FILE * yyin;
int ezparse(FILE* fd, const string target, const string dump) {
	yyin = fd;
	int ret = yyparse();
	if(ret) return ret;
	yyin = NULL;
	if(!dump.empty()) 
		s_vm.dump().dump(dump);
	if(target.empty()) s_vm.run();
	else s_vm.archive().save(target);
	return ret;
}

void yyerror (char const *s) {
	extern int yylineno;
	cout << "error (" << yylineno << "): " << s << endl;
}

