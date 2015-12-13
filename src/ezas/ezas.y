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

/** \page ezas
 * \section Brief
 * ezAssembler is an assembler language which is dedicated to ezVM.
 * \section Document
 * \subsection .import
 * Syntax\n
 * Operation\n
 * Description\n
 * Notes\n
 * Example\n
 * \subsection .entry
 * Syntax\n
 * Operation\n
 * Description\n
 * Notes\n
 * Example\n
 * \subsection .proc
 * Syntax\n
 * Operation\n
 * Description\n
 * Notes\n
 * Example\n
 * \subsection .memories
 * Syntax\n
 * Operation\n
 * Description\n
 * Notes\n
 * Example\n
 * \section Address
 * Syntax\n
 * [g|c|l|p|string]+:[0-9|string]+\n
 * Operation\n
 * Description\n
 * Address is described by segment and offset. 'g' stands for a global segment, 'c' stands for a constant segment, 'l' stands for a local segment, and 'p' stands for a parent local segment on the scope. 'string' comes from the name of .import.
 * Notes\n
 * Example\n
 * l:2\n
 * io:print\n
 * \section Instructions
 * The instructions are inspired by triVM instruction set. See 'TriVM intermediate language reference manual' by Neil Jonson.
 * \subsection add
 * Syntax\n
 * add rD [rC]?, [rS]+\n
 * Operation\n
 * rD = sum(rS)\n
 * rC = condition(sum(rS))\n
 * Description\n
 * The add instruction adds together the contents of rSs, placing the result in rD. Optionally, the condition codes of the esult can be placed in rC.\n
 * Notes\n
 * This instructions applied to both signed and unsigned values.\n
 * Example\n
 * add l:2, l:0 l:1\n
 * add l:3, 1 2 3 4
 * \subsection beq
 * Syntax\n
 * beq rS , label\n
 * Operation\n
 * if rS == EQU AL then goto label\n
 * Description\n
 * The beq instruction branches to the given label if the condition in rS is equal.\n
 * Notes\n
 * This instruction is applicable to both integer and floading-point comparisons.\n
 * Example\n
 * beq l:3 int_zero
 * \subsection blt
 * Syntax\n
 * blt rS, label\n
 * Operation\n
 * if rS == LESS-THAN then goto label\n
 * Description\n
 * The blt instruction branches to the given label if the condition in rS is less-than.\n
 * Notes\n
 * The unsigned equivalent is bbl. This instruction is also appplicable to floating point operations.
 * Example\n
 * blt l:3 int_neg
 * \subsection bra
 * Syntax\n
 * bra label\n
 * Operation\n
 * goto label\n
 * Description\n
 * Th bra instruction branches to ghe label identified either by the given label name.\n
 * Notes\n
 * Example\n
 * bra int_end
 * \subsection call
 * Syntax\n
 * call rS ( [rA]* ) [rR]*\n
 * Operation\n
 * call rS\n
 * Description\n
 * The call instruction calls the procedure identified by rS. Arguments, rA, are passed to the callee. On return (see the et instruction) results are assigned to the rRs.
 * Notes\n
 * The target address pointed to by rS must be the biginning of a procedure identified by a visibile procedure identifier.\n
 * Example\n
 * call io:print (stdout l:2 "=" l:0 "+" l:1)\n
 * \subsection mv
 * Syntax\n
 * mv [rD]+, [rS]+\n
 * Operation\n
 * rD = rS\n
 * Description\n
 * The mv instruction moves the value in rS into rD.
 * Notes\n
 * Example\n
 * mv l:0 l:1, 1 2
 * \subsection sub
 * Syntax\n
 * sub rD [rC]?, [rS]+\n
 * Operation\n
 * rD = rS[i] - rS[i+1] in {rS}\n
 * rC = condition(rS[i] - rS[i+1])\n
 * Description\n
 * The sub instruction subtacts the contents of rSs, placing the result in rD. Optionally, the condition codes of the result can be placed in rC.
 * Notes\n
 * Example\n
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

%token PROC ENTRY IMPORT CALL LD MV ADD SUB BRA BEQ BLT SYMBOL STRING NEWLINE INTEGER ADDRESS SYMBOLIC_ADDRESS MEMORIES LABEL

%type <s_value> PROC ENTRY CALL LD MV SYMBOL STRING NEWLINE LABEL
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
	| add
	| sub 
	| beq
	| blt
	| bra
	| label;

mv : MV addrs ',' vars {s_proc_current->mv(s_args_addr, s_args_var); s_args_addr.clear(); s_args_var.clear();};

ld : LD ADDRESS ',' var var {s_proc_current->ld(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));};

call : CALL fname '(' vars ')' addrs {s_proc_current->call(ezAddress($2.segment, $2.offset), s_args_var, s_args_addr); s_args_addr.clear(); s_args_var.clear();};

add : ADD var ',' vars {s_proc_current->add(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| ADD var var ',' vars {s_proc_current->add(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

sub : SUB var ',' vars {s_proc_current->sub(ezAddress($2.segment, $2.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}
	| SUB var var ',' vars {s_proc_current->sub(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), s_args_var); s_args_addr.clear(); s_args_var.clear();}

beq : BEQ var SYMBOL {s_proc_current->beq(ezAddress($2.segment, $2.offset), $3);}

blt : BLT var SYMBOL {s_proc_current->blt(ezAddress($2.segment, $2.offset), $3);}

bra : BRA SYMBOL {s_proc_current->bra($2);}

label : ':' SYMBOL {s_proc_current->label($2);}

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

