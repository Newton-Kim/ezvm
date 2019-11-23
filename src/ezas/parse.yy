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
#include "ezio.h"
#include "eaval.h"
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
 * [gcrp][0-9]+\n
 * \subsection ezas-address-operation Operation
 * \subsection ezas-address-description Description
 * Address is described by segment and offset. 'g' stands for a global segment, 'c' stands for a constant segment, 'l' stands for a local segment, and 'p' stands for a parent local segment on the scope. 'string' comes from the name of .import.
 * \subsection ezas-address-notes Notes
 * \subsection ezas-address-example Example
 * r2\n
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
 * add r2, r0 r1\n
 * add r3, 1 2 3 4
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
 * beq r3 int_zero
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
 * blt r3 int_neg
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
 * call io:print (stdout r2 "=" r0 "+" r1)\n
 * \subsection ezas-mv mv
 * \subsubsection ezas-mv-syntax Syntax
 * mv [rD]+, [rS]+\n
 * \subsubsection ezas-mv-operation Operation
 * rD = rS\n
 * \subsubsection ezas-mv-description Description
 * The mv instruction moves the value in rS into rD.
 * \subsubsection ezas-mv-notes Notes
 * \subsubsection ezas-mv-example Example
 * mv r0 r1, 1 2
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
 * sub r2, r0 r1\n
 * sub r2 r3, r0 r1
 */

int yylex();
void yyerror (char const *s);

using namespace std;
static ezVM s_vm;
static ezAsmProcedure* s_proc_current = NULL;
static ezAsmInstruction* s_instr_current = NULL;
static map<string, unsigned int> s_global;
static vector<ezAddress> s_args_addr;
static vector<ezAddress> s_args_var;
static size_t s_memories = 0;
static int s_scope = -1;
static int s_scpkey = -1;
static bool s_is_void = false;
%}

%token PROC ENTRY IMPORT
%token ADD AND BEQ BGE BLT BNE BRA CALL CMP DIV FGC LD LSL LSR MOD MUL MV NEG NOT OR POW RET SUB THD WAIT XOR
%token SYMBOL STRING NEWLINE INTEGER COMPLEX ADDRESS MEMORIES SCOPE SCOPE_KEY LABEL BOOLEAN

%type <s_value> PROC ENTRY CALL LD LSL LSR MV SYMBOL STRING NEWLINE LABEL
%type <i_value> INTEGER COMPLEX proc_meta
%type <a_value> ADDRESS fname var ret_addr
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
};

%start program

%%
program : import entry procs {
		if(s_proc_current) {
			if(s_instr_current) {
				s_proc_current->append_instruction(s_instr_current);
				delete s_instr_current;
			}
			s_proc_current->validate();
			delete s_proc_current;
		}
	};

import : %empty | IMPORT SYMBOL NEWLINE {
		//TODO:load a script ($2+'.ezas')
	};

entry : ENTRY SYMBOL NEWLINE { s_vm.assembler().entry($2); };

procs : proc
	| proc procs;

proc : PROC SYMBOL '(' INTEGER ')' NEWLINE {
		s_memories = 0;
		s_scope = -1;
		s_scpkey = -1;
	} proc_meta {
		if(s_proc_current) {
			if(s_instr_current) {
				s_proc_current->append_instruction(s_instr_current);
				delete s_instr_current;
			}
			s_proc_current->validate();
			delete s_proc_current;
		}
		s_instr_current = new ezAsmInstruction;
		s_proc_current = s_vm.assembler().new_proc($2, s_scpkey, s_scope);
		s_proc_current->args($4);
		s_proc_current->mems(s_memories);
		s_memories = 0;
		s_scope = -1;
		s_scpkey = -1;
	}
	codes {/*s_proc_current = NULL;*/};

proc_meta : %empty {$$ = 0;}
	| proc_meta SCOPE INTEGER NEWLINE {s_scope = $3;}
	| proc_meta SCOPE_KEY INTEGER NEWLINE {s_scpkey = $3;}
	| proc_meta MEMORIES INTEGER NEWLINE {s_memories = $3;};

codes : line NEWLINE | codes line NEWLINE;

line : %empty
        | add
	| and
	| beq
	| bge
	| blt
	| bne
	| bra
	| call
	| cmp
	| div
	| fgc
	| lsl
	| lsr
	| mod
	| mul
	| mv
	| neg
	| not
	| or 
	| pow 
	| ret 
	| sub 
	| thd
	| wait
	| xor 
	| label;

add : ADD var ',' var var {
		s_instr_current->add(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| ADD var var ',' var var {
		s_instr_current->add(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

and : AND var ',' var var {
		s_instr_current->bitwise_and(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| AND var var ',' var var {
		s_instr_current->bitwise_and(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

beq : BEQ var SYMBOL {s_instr_current->beq(ezAddress($2.segment, $2.offset), s_proc_current->label2index($3));}

bge : BGE var SYMBOL {s_instr_current->bge(ezAddress($2.segment, $2.offset), s_proc_current->label2index($3));}

blt : BLT var SYMBOL {s_instr_current->blt(ezAddress($2.segment, $2.offset), s_proc_current->label2index($3));}

bne : BNE var SYMBOL {s_instr_current->bne(ezAddress($2.segment, $2.offset), s_proc_current->label2index($3));}

bra : BRA SYMBOL {s_instr_current->bra(s_proc_current->label2index($2));}

ret_addr : %empty {s_is_void = true;} | ADDRESS {s_is_void = false; $$ = $1;}

call : CALL fname '(' vars ')' ret_addr {
		if (s_is_void) {
			s_instr_current->call(ezAddress($2.segment, $2.offset), s_args_var);
		} else {
			ezAddress ret = ezAddress($2.segment, $2.offset);
			s_instr_current->call(ezAddress($2.segment, $2.offset), s_args_var, ret);
		}
		s_args_var.clear();
	};

cmp : CMP var ',' var var {s_instr_current->cmp(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));}

div : DIV var ',' var var {
		s_instr_current->div(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| DIV var var ',' var var {
		s_instr_current->div(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

fgc : FGC {s_instr_current->fgc();}

lsl : LSL var ',' var ',' var {
		s_instr_current->lsl(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($6.segment, $6.offset));}
	| LSL var var ',' var ',' var {
		s_instr_current->lsl(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($7.segment, $7.offset));
	};

lsr : LSR var ',' var ',' var {
		s_instr_current->lsr(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($6.segment, $6.offset));
	}
	| LSR var var ',' var ',' var {
		s_instr_current->lsr(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($7.segment, $7.offset));
	};

mod : MOD var ',' var var {
		s_instr_current->mod(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| MOD var var ',' var var {
		s_instr_current->mod(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

mul : MUL var ',' var var {
		s_instr_current->mul(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| MUL var var ',' var var {
		s_instr_current->mul(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

mv : MV addrs ',' vars {s_instr_current->mv(s_args_addr, s_args_var); s_args_addr.clear(); s_args_var.clear();};

neg : NEG var ',' var {s_instr_current->neg(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset));}
	| NEG var var ',' var {s_instr_current->neg(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset));};

not : NOT var ',' var {s_instr_current->bitwise_not(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset));}
	| NOT var var ',' var {s_instr_current->bitwise_not(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset));};

or : OR var ',' var var {
		s_instr_current->bitwise_or(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| OR var var ',' var var {
		s_instr_current->bitwise_or(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

pow : POW var ',' var var {
		s_instr_current->powv(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| POW var var ',' var var {
		s_instr_current->powv(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

ret : RET {s_instr_current->ret();} | RET vars {s_instr_current->ret(s_args_var); s_args_var.clear();};

sub : SUB var ',' var var {
		s_instr_current->sub(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| SUB var var ',' var var {
		s_instr_current->sub(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

thd : THD fname '(' vars ';' ret_addr ')' ADDRESS{
		if(s_is_void) {
			s_instr_current->thd(ezAddress($2.segment, $2.offset), s_args_var, ezAddress($8.segment, $8.offset));
		} else {
			ezAddress ret = ezAddress($6.segment, $6.offset);
			s_instr_current->thd(ezAddress($2.segment, $2.offset), s_args_var, ret, ezAddress($8.segment, $8.offset));
		}
		s_args_var.clear();

	};

wait : WAIT ADDRESS {
		s_instr_current->wait(ezAddress($2.segment, $2.offset));
	};

xor : XOR var ',' var var {
		s_instr_current->bitwise_xor(ezAddress($2.segment, $2.offset), ezAddress($4.segment, $4.offset), ezAddress($5.segment, $5.offset));
	}
	| XOR var var ',' var var {
		s_instr_current->bitwise_xor(ezAddress($2.segment, $2.offset), ezAddress($3.segment, $3.offset), ezAddress($5.segment, $5.offset), ezAddress($6.segment, $6.offset));
	};

label : LABEL {s_proc_current->label($1, s_instr_current->size());};

fname : SYMBOL {$$.segment = EZ_ASM_SEGMENT_GLOBAL; $$.offset = s_vm.assembler().global($1);}
	| ADDRESS {$$ = $1;};

addrs : %empty | addrs ADDRESS {s_args_addr.push_back(ezAddress($2.segment, $2.offset));};

vars : %empty | vars var {s_args_var.push_back(ezAddress($2.segment, $2.offset));};

var : STRING {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant(new ezString($1));}
	| SYMBOL {$$.segment = EZ_ASM_SEGMENT_GLOBAL; $$.offset = s_vm.assembler().global($1);}
	| INTEGER {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant(new ezInteger($1));}
	| COMPLEX {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant(new ezComplex(complex<double>(0, $1)));}
	| BOOLEAN {$$.segment = EZ_ASM_SEGMENT_CONSTANT; $$.offset = s_vm.assembler().constant(new ezBool($1));}
	| ADDRESS {$$ = $1;};
%%

extern FILE * yyin;

int ezparse(FILE* fd, const string target, const string dump) {
	{
		char **symtab = NULL;
		ezObject **constant = NULL;
		ezIO::load(&symtab, &constant);
		s_vm.assembler().load_intrinsics(symtab, constant);
	}
	yyin = fd;
	int ret = yyparse();
	if(ret) return ret;
	yyin = NULL;
	if(!dump.empty()) {
		s_vm.dump(dump);
	}
	if(target.empty()) s_vm.run();
	return ret;
}

void yyerror (char const *s) {
	extern int yylineno;
	cout << "error (" << yylineno << "): " << s << endl;
}

