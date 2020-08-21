#include "ezvm/asm/ezasm_instr.h"
#include "ezvm/ezstack.h"

#include "asm/instruction/ezinst_binary_operation.h"
#include "asm/instruction/ezinst_mv.h"
#include "asm/instruction/ezinst_cmp.h"

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
  class ezInstrBeq : public ezInstruction {
  private:
    ezAddress m_cond;
    size_t m_offset;

  public:
    ezInstrBeq(const ezAddress cond, size_t offset)
        : m_cond(cond), m_offset(offset) {}
    void process(ezStackFrame &stk) { stk.beq(m_cond, m_offset); }
    void dump(ezFile &sink) {
      sink.print("beq");
      m_cond.dump(sink);
      sink.print(" %lu\n", m_offset);
    }
  };
  m_instruction.push_back(new ezInstrBeq(cond, offset));
}

void ezAsmInstruction::bge(const ezAddress cond, size_t offset) {
  class ezInstrBge : public ezInstruction {
  private:
    ezAddress m_cond;
    size_t m_offset;

  public:
    ezInstrBge(const ezAddress cond, size_t offset)
        : m_cond(cond), m_offset(offset) {}
    void process(ezStackFrame &stk) { stk.bge(m_cond, m_offset); }
    void dump(ezFile &sink) {
      sink.print("bge");
      m_cond.dump(sink);
      sink.print(" %lu\n", m_offset);
    }
  };
  m_instruction.push_back(new ezInstrBge(cond, offset));
}

void ezAsmInstruction::blt(const ezAddress cond, size_t offset) {
  class ezInstrBlt : public ezInstruction {
  private:
    ezAddress m_cond;
    size_t m_offset;

  public:
    ezInstrBlt(const ezAddress cond, size_t offset)
        : m_cond(cond), m_offset(offset) {}
    void process(ezStackFrame &stk) { stk.blt(m_cond, m_offset); }
    void dump(ezFile &sink) {
      sink.print("blt");
      m_cond.dump(sink);
      sink.print(" %lu\n", m_offset);
    }
  };
  m_instruction.push_back(new ezInstrBlt(cond, offset));
}

void ezAsmInstruction::bne(const ezAddress cond, size_t offset) {
  class ezInstrBlt : public ezInstruction {
  private:
    ezAddress m_cond;
    size_t m_offset;

  public:
    ezInstrBlt(const ezAddress cond, size_t offset)
        : m_cond(cond), m_offset(offset) {}
    void process(ezStackFrame &stk) { stk.bne(m_cond, m_offset); }
    void dump(ezFile &sink) {
      sink.print("bne");
      m_cond.dump(sink);
      sink.print(" %lu\n", m_offset);
    }
  };
  m_instruction.push_back(new ezInstrBlt(cond, offset));
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
  class ezInstrCall : public ezInstruction {
  private:
    ezAddress m_func;
    vector<ezAddress> m_args;
    ezAddress m_ret;

  public:
    ezInstrCall(const ezAddress &func, vector<ezAddress> &args, ezAddress &ret)
        : m_func(func), m_args(args), m_ret(ret) {}
    void process(ezStackFrame &stk) { stk.call(m_func, m_args, m_ret); }
    void dump(ezFile &sink) {
      sink.print("call");
      m_func.dump(sink);
      sink.print(",");
      if (m_args.empty())
        sink.print(" null");
      else
        for (vector<ezAddress>::iterator it = m_args.begin();
             it != m_args.end(); it++)
          (*it).dump(sink);
      sink.print(",");
      m_ret.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrCall(func, args, ret));
}

void ezAsmInstruction::call(const ezAddress &func, vector<ezAddress> &args) {
  class ezInstrCall : public ezInstruction {
  private:
    ezAddress m_func;
    vector<ezAddress> m_args;

  public:
    ezInstrCall(const ezAddress &func, vector<ezAddress> &args)
        : m_func(func), m_args(args) {}
    void process(ezStackFrame &stk) { stk.call(m_func, m_args); }
    void dump(ezFile &sink) {
      sink.print("call");
      m_func.dump(sink);
      sink.print(",");
      if (m_args.empty())
        sink.print(" null");
      else
        for (vector<ezAddress>::iterator it = m_args.begin();
             it != m_args.end(); it++)
          (*it).dump(sink);
      sink.print(", null");
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrCall(func, args));
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
    void process(ezStackFrame &stk) { stk.fgc(); }
    void dump(ezFile &sink) { sink.print("fgc\n"); }
  };
  m_instruction.push_back(new ezInstrFgc());
}

void ezAsmInstruction::lsl(const ezAddress dest, const ezAddress obj,
                           const ezAddress offset) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, obj, offset, "lsl",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->lsl(vl, vr);
      }));
}

void ezAsmInstruction::lsl(const ezAddress dest, const ezAddress cond,
                           const ezAddress obj, const ezAddress offset) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, obj, offset, "lsl",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->lsl(vl, vr);
      }));
}

void ezAsmInstruction::lsr(const ezAddress dest, const ezAddress obj,
                           const ezAddress offset) {
  m_instruction.push_back(new ezInstrBinaryOperation(
      m_alu, dest, obj, offset, "lsr",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->lsr(vl, vr);
      }));
}

void ezAsmInstruction::lsr(const ezAddress dest, const ezAddress cond,
                           const ezAddress obj, const ezAddress offset) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, obj, offset, "lsr",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->lsr(vl, vr);
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

void ezAsmInstruction::mv(vector<ezAddress> &dest, vector<ezAddress> &src) {
  m_instruction.push_back(new ezInstrMv(dest, src));
}

void ezAsmInstruction::mv(ezAddress &dest, ezAddress &src) {
  m_instruction.push_back(new ezInstrMv(dest, src));
}

void ezAsmInstruction::neg(const ezAddress dest, const ezAddress org) {
  class ezInstrNeg : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_org;

  public:
    ezInstrNeg(const ezAddress dest, const ezAddress org)
        : m_dest(dest), m_org(org) {}
    void process(ezStackFrame &stk) { stk.negate(m_dest, m_org); }
    void dump(ezFile &sink) {
      sink.print("neg");
      m_dest.dump(sink);
      sink.print(",");
      m_org.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrNeg(dest, org));
}

void ezAsmInstruction::neg(const ezAddress dest, const ezAddress cond,
                           const ezAddress org) {
  class ezInstrNeg : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_org;

  public:
    ezInstrNeg(const ezAddress dest, const ezAddress cond, const ezAddress org)
        : m_dest(dest), m_cond(cond), m_org(org) {}
    void process(ezStackFrame &stk) { stk.negate(m_dest, m_cond, m_org); }
    void dump(ezFile &sink) {
      sink.print("neg");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_org.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrNeg(dest, cond, org));
}

void ezAsmInstruction::bitwise_not(const ezAddress dest, const ezAddress org) {
  class ezInstrNot : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_org;

  public:
    ezInstrNot(const ezAddress dest, const ezAddress org)
        : m_dest(dest), m_org(org) {}
    void process(ezStackFrame &stk) { stk.b_not(m_dest, m_org); }
    void dump(ezFile &sink) {
      sink.print("not");
      m_dest.dump(sink);
      sink.print(",");
      m_org.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrNot(dest, org));
}

void ezAsmInstruction::bitwise_not(const ezAddress dest, const ezAddress cond,
                                   const ezAddress org) {
  class ezInstrNot : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_org;

  public:
    ezInstrNot(const ezAddress dest, const ezAddress cond, const ezAddress org)
        : m_dest(dest), m_cond(cond), m_org(org) {}
    void process(ezStackFrame &stk) { stk.b_not(m_dest, m_cond, m_org); }
    void dump(ezFile &sink) {
      sink.print("not");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_org.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrNot(dest, cond, org));
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
        return alu->power(vl, vr);
      }));
}

void ezAsmInstruction::powv(const ezAddress dest, const ezAddress cond,
                            const ezAddress &lsrc, const ezAddress &rsrc) {
  m_instruction.push_back(new ezInstrBinaryOperationWithCond(
      m_alu, dest, cond, lsrc, rsrc, "pow",
      [](ezALU *alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->power(vl, vr);
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
