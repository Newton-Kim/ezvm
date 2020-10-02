#include "ezvm/asm/ezasm_instr.h"
#include "ezvm/ezgc.h"
#include "ezvm/ezstack.h"

#include "asm/instruction/ezinst_binary_operation.h"
#include "asm/instruction/ezinst_call.h"
#include "asm/instruction/ezinst_cmp.h"
#include "asm/instruction/ezinst_conditional_bra.h"
#include "asm/instruction/ezinst_dup.h"
#include "asm/instruction/ezinst_get.h"
#include "asm/instruction/ezinst_mv.h"
#include "asm/instruction/ezinst_set.h"
#include "asm/instruction/ezinst_unary_operation.h"

using namespace std;

ezAsmInstruction::ezAsmInstruction(ezALU *alu) : m_alu(alu) {}
ezAsmInstruction::~ezAsmInstruction() {}

void ezAsmInstruction::add(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "add",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->add(vl, vr);
      }));
}

void ezAsmInstruction::add(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "add",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->add(vl, vr);
      }));
}

void ezAsmInstruction::bitwise_and(const ezAddress dest, const ezAddress &lsrc,
                                   const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "and",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->bitwise_and(vl, vr);
      }));
}

void ezAsmInstruction::bitwise_and(const ezAddress dest, const ezAddress cond,
                                   const ezAddress &lsrc,
                                   const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "and",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->bitwise_and(vl, vr);
      }));
}

void ezAsmInstruction::beq(const ezAddress cond, size_t offset) {
  m_instruction.push_back(new ezInstrConditionalBra(
      cond, offset, "beq", [](ezCondition *v) -> bool { return v->zero; }));
}

void ezAsmInstruction::bge(const ezAddress cond, size_t offset) {
  m_instruction.push_back(new ezInstrConditionalBra(
      cond, offset, "bge",
      [](ezCondition *v) -> bool { return (v->zero || !v->negative); }));
}

void ezAsmInstruction::blt(const ezAddress cond, size_t offset) {
  m_instruction.push_back(new ezInstrConditionalBra(
      cond, offset, "blt", [](ezCondition *v) -> bool { return v->negative; }));
}

void ezAsmInstruction::bne(const ezAddress cond, size_t offset) {
  m_instruction.push_back(new ezInstrConditionalBra(
      cond, offset, "beq", [](ezCondition *v) -> bool { return !v->zero; }));
}

void ezAsmInstruction::bra(size_t offset) {
  class ezInstrBra : public ezInstruction {
  private:
    size_t m_offset;

  public:
    ezInstrBra(size_t offset) : m_offset(offset) {}
    void process(ezStackFrame &stk) { stk.bra(m_offset); }
    void dump(ezFile &sink) {
      sink.print("bra");
      sink.print(" %lu\n", m_offset);
    }
  };
  m_instruction.push_back(new ezInstrBra(offset));
}

void ezAsmInstruction::call(const ezAddress &func, vector<ezAddress> &args,
                            ezAddress &ret) {
  m_instruction.push_back(new ezInstrCallFn(func, args, ret));
}

void ezAsmInstruction::call(const ezAddress &func, vector<ezAddress> &args) {
  m_instruction.push_back(new ezInstrCallVoid(func, args));
}

void ezAsmInstruction::cmp(const ezAddress &cond, const ezAddress &larg,
                           const ezAddress &rarg) {
  m_instruction.push_back(new ezInstrCmp(m_alu, cond, larg, rarg));
}

void ezAsmInstruction::div(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "div",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->divide(vl, vr);
      }));
}

void ezAsmInstruction::div(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "div",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->divide(vl, vr);
      }));
}

void ezAsmInstruction::fgc(void) {
  class ezInstrFgc : public ezInstruction {
  public:
    ezInstrFgc() {}
    void process(ezStackFrame &stk) { ezGC::instance().force(); }
    void dump(ezFile &sink) { sink.print("fgc\n"); }
  };
  m_instruction.push_back(new ezInstrFgc());
}

void ezAsmInstruction::lsl(const ezAddress dest, const ezAddress obj,
                           const ezAddress offset) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, obj, offset, "lsl",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return vl->lsl(vr);
      }));
}

void ezAsmInstruction::lsl(const ezAddress dest, const ezAddress cond,
                           const ezAddress obj, const ezAddress offset) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, obj, offset, "lsl",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return vl->lsl(vr);
      }));
}

void ezAsmInstruction::lsr(const ezAddress dest, const ezAddress obj,
                           const ezAddress offset) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, obj, offset, "lsr",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return vl->lsr(vr);
      }));
}

void ezAsmInstruction::lsr(const ezAddress dest, const ezAddress cond,
                           const ezAddress obj, const ezAddress offset) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, obj, offset, "lsr",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return vl->lsr(vr);
      }));
}

void ezAsmInstruction::mod(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "mod",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->modulate(vl, vr);
      }));
}

void ezAsmInstruction::mod(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "mod",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->modulate(vl, vr);
      }));
}

void ezAsmInstruction::mul(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "mul",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->multiply(vl, vr);
      }));
}

void ezAsmInstruction::mul(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "mul",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->multiply(vl, vr);
      }));
}

void ezAsmInstruction::mv(vector<ezAddress> &dest, vector<ezAddress> &src, char op) {
  m_instruction.push_back(new ezInstrMv(m_alu, dest, src, op));
}

void ezAsmInstruction::mv(ezAddress &dest, ezAddress &src, char op) {
  m_instruction.push_back(new ezInstrMv(m_alu, dest, src, op));
}

void ezAsmInstruction::neg(const ezAddress dest, const ezAddress org) {
  m_instruction.push_back(new ezInstrUnaryOperation(
      dest, org, "neg", [](ezValue *v) -> ezValue * { return v->negate(); }));
}

void ezAsmInstruction::neg(const ezAddress dest, const ezAddress cond,
                           const ezAddress org) {
  m_instruction.push_back(new ezInstrUnaryOperationWithCond(
      dest, cond, org, "neg",
      [](ezValue *v) -> ezValue * { return v->negate(); }));
}

void ezAsmInstruction::bitwise_not(const ezAddress dest, const ezAddress org) {
  m_instruction.push_back(
      new ezInstrUnaryOperation(dest, org, "not", [](ezValue *v) -> ezValue * {
        return v->bitwise_not();
      }));
}

void ezAsmInstruction::bitwise_not(const ezAddress dest, const ezAddress cond,
                                   const ezAddress org) {
  m_instruction.push_back(new ezInstrUnaryOperationWithCond(
      dest, cond, org, "not",
      [](ezValue *v) -> ezValue * { return v->bitwise_not(); }));
}

void ezAsmInstruction::bitwise_or(const ezAddress dest, const ezAddress &lsrc,
                                  const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "or",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->bitwise_or(vl, vr);
      }));
}

void ezAsmInstruction::bitwise_or(const ezAddress dest, const ezAddress cond,
                                  const ezAddress &lsrc,
                                  const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "or",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->bitwise_or(vl, vr);
      }));
}

void ezAsmInstruction::powv(const ezAddress dest, const ezAddress &lsrc,
                            const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "pow",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return vl->power(vr);
      }));
}

void ezAsmInstruction::powv(const ezAddress dest, const ezAddress cond,
                            const ezAddress &lsrc, const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "pow",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return vl->power(vr);
      }));
}

void ezAsmInstruction::ret(void) {
  class ezInstrRet : public ezInstruction {
  private:
    vector<ezAddress> m_rets;

  public:
    ezInstrRet() {}
    void process(ezStackFrame &stk) { stk.ret(m_rets); }
    void dump(ezFile &sink) { sink.print("ret\n"); }
  };
  m_instruction.push_back(new ezInstrRet());
}

void ezAsmInstruction::ret(vector<ezAddress> &src) {
  class ezInstrRet : public ezInstruction {
  private:
    vector<ezAddress> m_rets;

  public:
    ezInstrRet(vector<ezAddress> &src) : m_rets(src) {}
    void process(ezStackFrame &stk) { stk.ret(m_rets); }
    void dump(ezFile &sink) {
      sink.print("ret");
      if (m_rets.empty())
        sink.print(" null");
      else
        for (vector<ezAddress>::iterator it = m_rets.begin();
             it != m_rets.end(); it++)
          (*it).dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrRet(src));
}

void ezAsmInstruction::sub(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "sub",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->subtract(vl, vr);
      }));
}

void ezAsmInstruction::sub(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "sub",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->subtract(vl, vr);
      }));
}

/*
void ezAsmInstruction::teq(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.teq(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezInstruction &inst) {
        ezDump::instance()->dump(sink, EZ_OP_TEQ, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::teq(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.teq(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezInstruction &inst) {
        ezDump::instance()->dump(sink, EZ_OP_TEQ, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::tge(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tge(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezInstruction &inst) {
        ezDump::instance()->dump(sink, EZ_OP_TGE, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::tge(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tge(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezInstruction &inst) {
        ezDump::instance()->dump(sink, EZ_OP_TGE, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}
*/

void ezAsmInstruction::thd(const ezAddress &func, vector<ezAddress> &args,
                           ezAddress &ret, const ezAddress &handle) {
  class ezInstrThd : public ezInstruction {
  private:
    ezAddress m_func;
    vector<ezAddress> m_args;
    ezAddress m_ret;
    ezAddress m_handle;

  public:
    ezInstrThd(const ezAddress &func, vector<ezAddress> &args, ezAddress &ret,
               const ezAddress &handle)
        : m_func(func), m_args(args), m_ret(ret), m_handle(handle) {}
    void process(ezStackFrame &stk) {
      stk.thd(m_func, m_args, m_ret, m_handle);
    }
    void dump(ezFile &sink) {
      sink.print("thd");
      m_func.dump(sink);
      if (m_args.empty())
        sink.print(" null");
      else
        for (vector<ezAddress>::iterator it = m_args.begin();
             it != m_args.end(); it++)
          (*it).dump(sink);
      sink.print(",");
      m_ret.dump(sink);
      m_handle.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrThd(func, args, ret, handle));
}

void ezAsmInstruction::thd(const ezAddress &func, vector<ezAddress> &args,
                           const ezAddress &handle) {
  class ezInstrThd : public ezInstruction {
  private:
    ezAddress m_func;
    vector<ezAddress> m_args;
    ezAddress m_handle;

  public:
    ezInstrThd(const ezAddress &func, vector<ezAddress> &args,
               const ezAddress &handle)
        : m_func(func), m_args(args), m_handle(handle) {}
    void process(ezStackFrame &stk) { stk.thd(m_func, m_args, m_handle); }
    void dump(ezFile &sink) {
      sink.print("thd");
      m_func.dump(sink);
      if (m_args.empty())
        sink.print(" null");
      else
        for (vector<ezAddress>::iterator it = m_args.begin();
             it != m_args.end(); it++)
          (*it).dump(sink);
      sink.print(", null, ");
      m_handle.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrThd(func, args, handle));
}

/*
void ezAsmInstruction::tlt(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tlt(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezInstruction &inst) {
        ezDump::instance()->dump(sink, EZ_OP_TLT, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::tlt(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tlt(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezInstruction &inst) {
        ezDump::instance()->dump(sink, EZ_OP_TLT, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}

void ezAsmInstruction::tne(const ezAddress dest, const ezAddress &lsrc,
                         const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tne(inst.dests[0], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezInstruction &inst) {
        ezDump::instance()->dump(sink, EZ_OP_TNE, inst.dests, inst.srcs);
      },
      dest, lsrc, rsrc);
}

void ezAsmInstruction::tne(const ezAddress dest, const ezAddress cond,
                         const ezAddress &lsrc, const ezAddress &rsrc) {
  instruction_with_binary_arguments(
      [](ezStackFrame &stk, ezInstruction &inst) {
        stk.tne(inst.dests[0], inst.dests[1], inst.srcs[0], inst.srcs[1]);
      },
      [](ezFile &sink, ezInstruction &inst) {
        ezDump::instance()->dump(sink, EZ_OP_TNE, inst.dests, inst.srcs);
      },
      dest, cond, lsrc, rsrc);
}
*/

void ezAsmInstruction::wait(const ezAddress &handle) {
  class ezInstrWait : public ezInstruction {
  private:
    ezAddress m_handle;

  public:
    ezInstrWait(const ezAddress handle) : m_handle(handle) {}
    void process(ezStackFrame &stk) { stk.wait(m_handle); }
    void dump(ezFile &sink) {
      sink.print("wait");
      m_handle.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrWait(handle));
}

void ezAsmInstruction::bitwise_xor(const ezAddress dest, const ezAddress &lsrc,
                                   const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, lsrc, rsrc, "xor",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->bitwise_xor(vl, vr);
      }));
}

void ezAsmInstruction::bitwise_xor(const ezAddress dest, const ezAddress cond,
                                   const ezAddress &lsrc,
                                   const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "xor",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->bitwise_xor(vl, vr);
      }));
}

void ezAsmInstruction::user_command(ezInstruction *inst) {
  m_instruction.push_back(inst);
}

void ezAsmInstruction::get(const ezAddress dest, const ezAddress container,
                           const ezAddress member) {
  m_instruction.push_back(new ezInstrGet(dest, container, member));
}

void ezAsmInstruction::get(const ezAddress dest, const ezAddress container,
                           vector<ezAddress> member) {
  m_instruction.push_back(new ezInstrGetByArray(dest, container, member));
}

void ezAsmInstruction::set(const ezAddress container, const ezAddress member,
                           const ezAddress val) {
  m_instruction.push_back(new ezInstrSet(container, member, val));
}

void ezAsmInstruction::set(const ezAddress container, vector<ezAddress> member,
                           const ezAddress val) {
  m_instruction.push_back(new ezInstrSetByArray(container, member, val));
}

void ezAsmInstruction::dup(const ezAddress dest, const ezAddress src) {
  m_instruction.push_back(new ezInstrDup(dest, src));
}
