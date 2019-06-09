#include "ezvm/asm/ezasm_instr.h"
#include "ezvm/ezdump.h"
#include "ezvm/ezstack.h"

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

ezAsmInstruction::ezAsmInstruction(){}
ezAsmInstruction::~ezAsmInstruction(){}

void ezAsmInstruction::instruction_with_binary_arguments(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress dest, const ezAddress obj, const ezAddress offset) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->srcs.push_back(obj);
  inst->srcs.push_back(offset);
  inst->dests.push_back(dest);
  m_instruction.push_back(inst);
}

void ezAsmInstruction::instruction_with_binary_arguments(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress dest, const ezAddress cond, const ezAddress obj,
    const ezAddress offset) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->srcs.push_back(obj);
  inst->srcs.push_back(offset);
  inst->dests.push_back(dest);
  inst->dests.push_back(cond);
  m_instruction.push_back(inst);
}

void ezAsmInstruction::instruction_with_unary_argument(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress dest, const ezAddress org) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->srcs.push_back(org);
  inst->dests.push_back(dest);
  m_instruction.push_back(inst);
}

void ezAsmInstruction::instruction_with_unary_argument(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress dest, const ezAddress cond, const ezAddress org) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->srcs.push_back(org);
  inst->dests.push_back(dest);
  inst->dests.push_back(cond);
  m_instruction.push_back(inst);
}

void ezAsmInstruction::branch_instruction(
    function<void(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump,
    const ezAddress cond, size_t offset) {
  ezInstruction *inst = new ezInstruction(func, dump);
  inst->arg = cond;
  inst->offset = offset;
  m_instruction.push_back(inst);
}

void ezAsmInstruction::add(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.add(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_ADD, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::add(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.add(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_ADD, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::bitwise_and(const ezAddress dest, const ezAddress &lsrc,
                                 const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.b_and(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_AND, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::bitwise_and(const ezAddress dest, const ezAddress cond,
                                 const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.b_and(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_AND, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::beq(const ezAddress cond, size_t offset) {
  branch_instruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.beq(inst.arg, inst.offset);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BEQ, inst.arg, inst.offset);
      },
      cond, offset);
}

void ezAsmInstruction::bge(const ezAddress cond, size_t offset) {
  branch_instruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.bge(inst.arg, inst.offset);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BGE, inst.arg, inst.offset);
      },
      cond, offset);
}

void ezAsmInstruction::blt(const ezAddress cond, size_t offset) {
  branch_instruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.blt(inst.arg, inst.offset);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BLT, inst.arg, inst.offset);
      },
      cond, offset);
}

void ezAsmInstruction::bne(const ezAddress cond, size_t offset) {
  branch_instruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.bne(inst.arg, inst.offset);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BNE, inst.arg, inst.offset);
      },
      cond, offset);
}

void ezAsmInstruction::bra(size_t offset) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.bra(inst.offset); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_BRA, inst.offset);
      });
  inst->offset = offset;
  m_instruction.push_back(inst);
}

void ezAsmInstruction::call(const ezAddress &func, vector<ezAddress> &args,
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
  m_instruction.push_back(inst);
}

void ezAsmInstruction::cmp(const ezAddress &cond, const ezAddress &larg,
                         const ezAddress &rarg) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.cmp(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_CMP, inst.dests, inst.srcs);
      });
  inst->dests.push_back(cond);
  inst->srcs.push_back(larg);
  inst->srcs.push_back(rarg);
  m_instruction.push_back(inst);
}

void ezAsmInstruction::div(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.div(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_DIV, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::div(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.div(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_DIV, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::fgc(void) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.fgc(); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_FGC);
      });
  m_instruction.push_back(inst);
}

void ezAsmInstruction::lsl(const ezAddress dest, const ezAddress obj,
                         const ezAddress offset) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.lsl(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_LSL, inst.dests, inst.srcs);
      },
      dest, obj, offset);
}

void ezAsmInstruction::lsl(const ezAddress dest, const ezAddress cond,
                         const ezAddress obj, const ezAddress offset) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.lsl(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_LSL, inst.dests, inst.srcs);
      },
      dest, cond, obj, offset);
}

void ezAsmInstruction::lsr(const ezAddress dest, const ezAddress obj,
                         const ezAddress offset) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.lsr(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_LSR, inst.dests, inst.srcs);
      },
      dest, obj, offset);
}

void ezAsmInstruction::lsr(const ezAddress dest, const ezAddress cond,
                         const ezAddress obj, const ezAddress offset) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.lsr(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_LSR, inst.dests, inst.srcs);
      },
      dest, cond, obj, offset);
}

void ezAsmInstruction::mod(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.mod(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MOD, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::mod(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.mod(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MOD, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::mul(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.mul(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MUL, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::mul(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.mul(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MUL, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::mv(vector<ezAddress> &dest, vector<ezAddress> &src) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.mv(inst.dests, inst.srcs);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MV, inst.dests, inst.srcs);
      });
  inst->srcs = src;
  inst->dests = dest;
  m_instruction.push_back(inst);
}

void ezAsmInstruction::mv(ezAddress &dest, ezAddress &src) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.mv(inst.dests, inst.srcs);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_MV, inst.dests, inst.srcs);
      });
  inst->srcs.push_back(src);
  inst->dests.push_back(dest);
  m_instruction.push_back(inst);
}

void ezAsmInstruction::neg(const ezAddress dest, const ezAddress org) {
  instruction_with_unary_argument(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.negate(inst.dests[0], inst.srcs[0]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_NEG, inst.dests, inst.srcs);
      },
      dest, org);
}

void ezAsmInstruction::neg(const ezAddress dest, const ezAddress cond,
                         const ezAddress org) {
  instruction_with_unary_argument(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.negate(inst.dests[0], inst.dests[1], inst.srcs[0]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_NEG, inst.dests, inst.srcs);
      },
      dest, cond, org);
}

void ezAsmInstruction::bitwise_not(const ezAddress dest, const ezAddress org) {
  instruction_with_unary_argument(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.b_not(inst.dests[0], inst.srcs[0]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_NOT, inst.dests, inst.srcs);
      },
      dest, org);
}

void ezAsmInstruction::bitwise_not(const ezAddress dest, const ezAddress cond,
                                 const ezAddress org) {
  instruction_with_unary_argument(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.b_not(inst.dests[0], inst.dests[1], inst.srcs[0]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_NOT, inst.dests, inst.srcs);
      },
      dest, cond, org);
}

void ezAsmInstruction::bitwise_or(const ezAddress dest, const ezAddress &lsrc,
                                const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.b_or(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_OR, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::bitwise_or(const ezAddress dest, const ezAddress cond,
                                const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.b_or(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_OR, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::powv(const ezAddress dest, const ezAddress &lsrc,
                          const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.pwr(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_POW, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::powv(const ezAddress dest, const ezAddress cond,
                          const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.pwr(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_POW, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::ret(void) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.ret(inst.srcs); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_RET);
      });
  m_instruction.push_back(inst);
}

void ezAsmInstruction::ret(vector<ezAddress> &src) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.ret(inst.srcs); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_RET, inst.srcs);
      });
  inst->srcs = src;
  m_instruction.push_back(inst);
}

void ezAsmInstruction::sub(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.sub(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_SUB, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::sub(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.sub(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_SUB, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

/*
void ezAsmInstruction::teq(const ezAddress dest, const ezAddress &lsrc,
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

void ezAsmInstruction::teq(const ezAddress dest, const ezAddress cond,
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

void ezAsmInstruction::tge(const ezAddress dest, const ezAddress &lsrc,
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

void ezAsmInstruction::tge(const ezAddress dest, const ezAddress cond,
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
*/

void ezAsmInstruction::thd(const ezAddress &func, vector<ezAddress> &args,
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
  m_instruction.push_back(inst);
}

/*
void ezAsmInstruction::tlt(const ezAddress dest, const ezAddress &lsrc,
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

void ezAsmInstruction::tlt(const ezAddress dest, const ezAddress cond,
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

void ezAsmInstruction::tne(const ezAddress dest, const ezAddress &lsrc,
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

void ezAsmInstruction::tne(const ezAddress dest, const ezAddress cond,
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
*/

void ezAsmInstruction::wait(const ezAddress &handle) {
  ezInstruction *inst = new ezInstruction(
      [](ezStackFrame &stk, ezInstruction &inst) { stk.wait(inst.arg); },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_WAIT, inst.arg);
      });
  inst->arg = handle;
  m_instruction.push_back(inst);
}

void ezAsmInstruction::bitwise_xor(const ezAddress dest, const ezAddress &lsrc,
                                 const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.b_xor(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_XOR, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::bitwise_xor(const ezAddress dest, const ezAddress cond,
                                 const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.b_xor(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezDump &dump, ezInstruction &inst) {
        dump.dump(sink, EZ_OP_XOR, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::user_command(ezInstruction *inst) {
  m_instruction.push_back(inst);
}

