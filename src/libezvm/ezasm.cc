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
#include "ezvm/ezasm.h"
#include "ezvm/ezdump.h"
#include "ezvm/ezmemory.h"
#include "ezvm/ezstack.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

#define EZ_OP_ADD "add"
#define EZ_OP_AND "and"
#define EZ_OP_BEQ "beq"
#define EZ_OP_BGE "bge"
#define EZ_OP_BLT "blt"
#define EZ_OP_BNE "bne"
#define EZ_OP_BRA "bra"
#define EZ_OP_CALL "call"
#define EZ_OP_CMP "cmp"
#define EZ_OP_DIV "div"
#define EZ_OP_FGC "fgc"
#define EZ_OP_LSL "lsl"
#define EZ_OP_LSR "lsr"
#define EZ_OP_MOD "mod"
#define EZ_OP_MUL "mul"
#define EZ_OP_MV "mv"
#define EZ_OP_NEG "neg"
#define EZ_OP_NOT "not"
#define EZ_OP_OR "or"
#define EZ_OP_POW "pow"
#define EZ_OP_RET "ret"
#define EZ_OP_SUB "sub"
#define EZ_OP_TEQ "teq"
#define EZ_OP_TGE "tge"
#define EZ_OP_THD "thd"
#define EZ_OP_TLT "tlt"
#define EZ_OP_TNE "tne"
#define EZ_OP_WAIT "wait"
#define EZ_OP_XOR "xor"

ezAsmProcedure::ezAsmProcedure(ezFunction *carousel) : m_carousel(carousel) {}

void ezAsmProcedure::args(size_t args) { m_carousel->nargs = args; }

void ezAsmProcedure::mems(size_t mems) { m_carousel->nmems = mems; }

size_t ezAsmProcedure::local(const string value) {
  // TODO:how to increase or set nmems and hash value.
}

void ezAsmProcedure::call(const ezAddress &func, vector<ezAddress> &args,
                          vector<ezAddress> &rets) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.call(inst.arg, inst.srcs, inst.dests);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_CALL, inst.arg, inst.srcs, inst.dests);
      });
  inst->arg = func;
  inst->srcs = args;
  inst->dests = rets;
  m_carousel->instruction.push_back(inst);
}
void ezAsmProcedure::thd(const ezAddress &func, vector<ezAddress> &args,
                         vector<ezAddress> &rets, const ezAddress &handle) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.thd(inst.arg, inst.srcs, inst.dests, inst.handle);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_THD, inst.arg, inst.srcs, inst.dests,
                  inst.handle);
      });
  inst->arg = func;
  inst->srcs = args;
  inst->dests = rets;
  inst->handle = handle;
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::wait(const ezAddress &handle) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.wait(inst.arg); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_WAIT, inst.arg);
      });
  inst->arg = handle;
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::cmp(const ezAddress &cond, const ezAddress &larg,
                         const ezAddress &rarg) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_COMPARISON, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_CMP, inst.dests, inst.srcs);
      });
  inst->dests.push_back(cond);
  inst->srcs.push_back(larg);
  inst->srcs.push_back(rarg);
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::mv(vector<ezAddress> &dest, vector<ezAddress> &src) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.mv(inst.dests, inst.srcs);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MV, inst.dests, inst.srcs);
      });
  inst->srcs = src;
  inst->dests = dest;
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::mv(ezAddress &dest, ezAddress &src) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.mv(inst.dests, inst.srcs);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MV, inst.dests, inst.srcs);
      });
  inst->srcs.push_back(src);
  inst->dests.push_back(dest);
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::instruction_with_binary_arguments(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress dest, const ezAddress obj, const ezAddress offset) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->srcs.push_back(obj);
  inst->srcs.push_back(offset);
  inst->dests.push_back(dest);
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::instruction_with_binary_arguments(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress dest, const ezAddress cond, const ezAddress obj,
    const ezAddress offset) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->srcs.push_back(obj);
  inst->srcs.push_back(offset);
  inst->dests.push_back(dest);
  inst->dests.push_back(cond);
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::lsl(const ezAddress dest, const ezAddress obj,
                         const ezAddress offset) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_LSL, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_LSL, inst.dests, inst.srcs);
      },
      dest, obj, offset);
}

void ezAsmProcedure::lsl(const ezAddress dest, const ezAddress cond,
                         const ezAddress obj, const ezAddress offset) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_LSL, inst.dests[0], inst.dests[1],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_LSL, inst.dests, inst.srcs);
      },
      dest, cond, obj, offset);
}

void ezAsmProcedure::lsr(const ezAddress dest, const ezAddress obj,
                         const ezAddress offset) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_LSR, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_LSR, inst.dests, inst.srcs);
      },
      dest, obj, offset);
}

void ezAsmProcedure::lsr(const ezAddress dest, const ezAddress cond,
                         const ezAddress obj, const ezAddress offset) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_LSR, inst.dests[0], inst.dests[1],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_LSR, inst.dests, inst.srcs);
      },
      dest, cond, obj, offset);
}

void ezAsmProcedure::add(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_ADDITION, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_ADD, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::add(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_ADDITION, inst.dests[0], inst.dests[1],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_ADD, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::div(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_DIVISION, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_DIV, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::div(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_DIVISION, inst.dests[0], inst.dests[1],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_DIV, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::fgc(void) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.fgc(); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_FGC);
      });
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::mod(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_MODULATION, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MOD, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::mod(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_MODULATION, inst.dests[0],
                    inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MOD, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::mul(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_MULTIPLICATION, inst.dests[0],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MUL, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::mul(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_MULTIPLICATION, inst.dests[0],
                    inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MUL, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::instruction_with_unary_argument(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress dest, const ezAddress org) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->srcs.push_back(org);
  inst->dests.push_back(dest);
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::instruction_with_unary_argument(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress dest, const ezAddress cond, const ezAddress org) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->srcs.push_back(org);
  inst->dests.push_back(dest);
  inst->dests.push_back(cond);
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::neg(const ezAddress dest, const ezAddress org) {
  instruction_with_unary_argument(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_UNARY_OPERATION_NEGATE, inst.dests[0], inst.srcs[0]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_NEG, inst.dests, inst.srcs);
      },
      dest, org);
}

void ezAsmProcedure::neg(const ezAddress dest, const ezAddress cond,
                         const ezAddress org) {
  instruction_with_unary_argument(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_UNARY_OPERATION_NEGATE, inst.dests[0], inst.dests[1],
                    inst.srcs[0]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_NEG, inst.dests, inst.srcs);
      },
      dest, cond, org);
}

void ezAsmProcedure::bitwise_not(const ezAddress dest, const ezAddress org) {
  instruction_with_unary_argument(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_UNARY_OPERATION_NOT, inst.dests[0], inst.srcs[0]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_NOT, inst.dests, inst.srcs);
      },
      dest, org);
}

void ezAsmProcedure::bitwise_not(const ezAddress dest, const ezAddress cond,
                                 const ezAddress org) {
  instruction_with_unary_argument(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_UNARY_OPERATION_NOT, inst.dests[0], inst.dests[1],
                    inst.srcs[0]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_NOT, inst.dests, inst.srcs);
      },
      dest, cond, org);
}

void ezAsmProcedure::powv(const ezAddress dest, const ezAddress &lsrc,
                          const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_POW, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_POW, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::powv(const ezAddress dest, const ezAddress cond,
                          const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_POW, inst.dests[0], inst.dests[1],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_POW, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::ret(void) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.ret(inst.srcs); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_RET);
      });
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::ret(vector<ezAddress> &src) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.ret(inst.srcs); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_RET, inst.srcs);
      });
  inst->srcs = src;
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::sub(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_SUBTRACTION, inst.dests[0],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_SUB, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::sub(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_SUBTRACTION, inst.dests[0],
                    inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_SUB, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::bitwise_and(const ezAddress dest, const ezAddress &lsrc,
                                 const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_AND, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_AND, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::bitwise_and(const ezAddress dest, const ezAddress cond,
                                 const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_AND, inst.dests[0], inst.dests[1],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_AND, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::bitwise_or(const ezAddress dest, const ezAddress &lsrc,
                                const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_OR, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_OR, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::bitwise_or(const ezAddress dest, const ezAddress cond,
                                const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_OR, inst.dests[0], inst.dests[1],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_OR, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}
void ezAsmProcedure::teq(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.teq(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_TEQ, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::teq(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.teq(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_TEQ, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::tge(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tge(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_TGE, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::tge(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tge(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_TGE, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::tlt(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tlt(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_TLT, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::tlt(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tlt(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_TLT, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::tne(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tne(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_TNE, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::tne(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tne(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_TNE, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmProcedure::bitwise_xor(const ezAddress dest, const ezAddress &lsrc,
                                 const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_XOR, inst.dests[0], inst.srcs[0],
                    inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_XOR, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmProcedure::bitwise_xor(const ezAddress dest, const ezAddress cond,
                                 const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.operate(EZ_BINARY_OPERATION_XOR, inst.dests[0], inst.dests[1],
                    inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_XOR, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

size_t ezAsmProcedure::label2index(string label) {
  if (!m_carousel->jmptbl.exist(label))
    m_carousel->jmptbl.add(label, 0);
  return m_carousel->jmptbl[label];
}

void ezAsmProcedure::branch_instruction(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress cond, string label) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->arg = cond;
  inst->offset = label2index(label);
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::beq(const ezAddress cond, string label) {
  branch_instruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.beq(inst.arg, inst.offset);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BEQ, inst.arg, inst.offset);
      },
      cond, label);
}

void ezAsmProcedure::bge(const ezAddress cond, string label) {
  branch_instruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.bge(inst.arg, inst.offset);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BGE, inst.arg, inst.offset);
      },
      cond, label);
}

void ezAsmProcedure::blt(const ezAddress cond, string label) {
  branch_instruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.blt(inst.arg, inst.offset);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BLT, inst.arg, inst.offset);
      },
      cond, label);
}

void ezAsmProcedure::bne(const ezAddress cond, string label) {
  branch_instruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.bne(inst.arg, inst.offset);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BNE, inst.arg, inst.offset);
      },
      cond, label);
}

void ezAsmProcedure::bra(string label) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.bra(inst.offset); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BRA, inst.offset);
      });
  inst->offset = label2index(label);
  m_carousel->instruction.push_back(inst);
}

void ezAsmProcedure::label(string name) {
  m_carousel->jmptbl.add(name, m_carousel->instruction.size());
}

void ezAsmProcedure::user_command(ezInstruction* inst) {
  m_carousel->instruction.push_back(inst);
}

ezASM::ezASM(ezAddress &entry)
    : m_entry(entry), m_constants(ezMemory::instance().constants()),
      m_globals(ezMemory::instance().globals()), m_gc(ezGC::instance()) {}

ezASM::~ezASM() {}

void ezASM::load_intrinsics(char **symtab, ezValue **constants) {
  if (!symtab || !constants)
    throw runtime_error("entry is not found");
  map<string, size_t> *offset_symtab = new map<string, size_t>;
  /*TODO:should it be put to constant?*/
  for (size_t i = 0; constants[i] && symtab[i]; i++) {
    size_t offset = m_globals.add(symtab[i], constants[i]);
    m_gc.add((ezGCObject *)constants[i]);
  }
}

void ezASM::entry(const string entry) { m_entry_string = entry; }

void ezASM::reset(const string name) { m_globals.reset(name); }

ezAsmProcedure *ezASM::new_proc(const string name, int scpkey, int scope) {
  ezTable<string, ezValue *> *p_scope = NULL, *p_scpkey = NULL;
  if (m_globals.exist(name) && !m_globals.is_null(name))
    throw runtime_error("global symbol " + name + " already exists");
  stringstream ss;
  if (scope >= 0) {
    if (m_scopes.end() == m_scopes.find(scope)) {
      ss << "scope[" << scope << "] doesn't exist";
      throw runtime_error(ss.str());
    }
    p_scope = m_scopes[scope];
  }
  if (scpkey >= 0) {
    if (m_scopes.end() != m_scopes.find(scpkey)) {
      ss << "scope[" << scpkey << "] already exist";
      throw runtime_error(ss.str());
    }
    m_scopes[scpkey] = new ezTable<string, ezValue *>;
    m_gc.add((ezGCObject *)m_scopes[scpkey]);
    p_scpkey = m_scopes[scpkey];
  }
  ezFunction *carousel = new ezFunction(p_scpkey, p_scope);
  size_t offset = m_globals.add(name, carousel);
  m_gc.add((ezGCObject *)carousel);
  if (name == m_entry_string) {
    m_entry.segment = EZ_ASM_SEGMENT_GLOBAL;
    m_entry.offset = offset;
  }
  return new ezAsmProcedure(carousel);
}

size_t ezASM::global(const string value) {
  //	vector<ezValue*>* offset = m_globals[0];
  if (!m_globals.exist(value))
    m_globals.add(value, NULL);
  //    throw runtime_error("global symbol " + value + " is not found");
  return m_globals[value];
}

bool ezASM::is_global(const string value) { return m_globals.exist(value); }

size_t ezASM::constant_null(void) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue *v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_NULL)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue *v = ezNull::instance();
  m_constants.push_back(v);
  return idx;
}

size_t ezASM::constant(const char *arg) {
  string value = arg;
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue *v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_STRING && ((ezString *)v)->value == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue *v = new ezString(value);
  m_constants.push_back(v);
  m_gc.add((ezGCObject *)v);
  return idx;
}

size_t ezASM::constant(const int value) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue *v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_INTEGER && ((ezInteger *)v)->value == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue *v = new ezInteger(value);
  m_constants.push_back(v);
  m_gc.add((ezGCObject *)v);
  return idx;
}

size_t ezASM::constant(const bool value) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue *v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_BOOL && ((ezBool *)v)->value == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue *v = new ezBool(value);
  m_constants.push_back(v);
  m_gc.add((ezGCObject *)v);
  return idx;
}

size_t ezASM::constant(const double value) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue *v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_FLOAT && ((ezFloat *)v)->value == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue *v = new ezFloat(value);
  m_constants.push_back(v);
  m_gc.add((ezGCObject *)v);
  return idx;
}

size_t ezASM::constant(const complex<double> value) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue *v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_COMPLEX && ((ezComplex *)v)->value == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue *v = new ezComplex(value);
  m_constants.push_back(v);
  m_gc.add((ezGCObject *)v);
  return idx;
}
