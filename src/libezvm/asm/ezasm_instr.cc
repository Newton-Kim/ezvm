#include "ezvm/asm/ezasm_instr.h"
#include "ezvm/ezstack.h"

using namespace std;

ezAsmInstruction::ezAsmInstruction() {}
ezAsmInstruction::~ezAsmInstruction() {}

void ezAsmInstruction::add(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  class ezInstrAdd : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrAdd(const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.add(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("add");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrAdd(dest, lsrc, rsrc));
}

void ezAsmInstruction::add(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  class ezInstrAddWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrAddWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.add(m_dest, m_cond, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("add");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrAddWithCond(dest, cond, lsrc, rsrc));
}

void ezAsmInstruction::bitwise_and(const ezAddress dest, const ezAddress &lsrc,
                                   const ezAddress &rsrc) {
  class ezInstrAnd : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrAnd(const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.b_and(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("and");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrAnd(dest, lsrc, rsrc));
}

void ezAsmInstruction::bitwise_and(const ezAddress dest, const ezAddress cond,
                                   const ezAddress &lsrc,
                                   const ezAddress &rsrc) {
  class ezInstrAndWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrAndWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) {
      stk.b_and(m_dest, m_cond, m_lsrc, m_rsrc);
    }
    void dump(ezFile &sink) {
      sink.print("and");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrAndWithCond(dest, cond, lsrc, rsrc));
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
                            vector<ezAddress> &rets) {
  class ezInstrCall : public ezInstruction {
  private:
    ezAddress m_func;
    vector<ezAddress> m_args;
    vector<ezAddress> m_rets;

  public:
    ezInstrCall(const ezAddress &func, vector<ezAddress> &args,
                vector<ezAddress> &rets)
        : m_func(func), m_args(args), m_rets(rets) {}
    void process(ezStackFrame &stk) { stk.call(m_func, m_args, m_rets); }
    void dump(ezFile &sink) {
      sink.print("call");
      m_func.dump(sink);
      if(m_args.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_args.begin() ; it != m_args.end() ; it++)
          (*it).dump(sink);
      sink.print(",");
      if(m_rets.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_rets.begin() ; it != m_rets.end() ; it++)
          (*it).dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrCall(func, args, rets));
}

void ezAsmInstruction::cmp(const ezAddress &cond, const ezAddress &larg,
                           const ezAddress &rarg) {
  class ezInstrCmp : public ezInstruction {
  private:
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrCmp(const ezAddress cond, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.cmp(m_cond, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("cmp");
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
    }
  };
  m_instruction.push_back(new ezInstrCmp(cond, larg, rarg));
}

void ezAsmInstruction::div(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  class ezInstrDiv : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrDiv(const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.div(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("div");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrDiv(dest, lsrc, rsrc));
}

void ezAsmInstruction::div(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  class ezInstrDivWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrDivWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.div(m_dest, m_cond, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("div");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrDivWithCond(dest, cond, lsrc, rsrc));
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
  class ezInstrLsl : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_obj;
    ezAddress m_offset;

  public:
    ezInstrLsl(const ezAddress dest, const ezAddress &obj,
               const ezAddress &offset)
        : m_dest(dest), m_obj(obj), m_offset(offset) {}
    void process(ezStackFrame &stk) { stk.lsl(m_dest, m_obj, m_offset); }
    void dump(ezFile &sink) {
      sink.print("lsl");
      m_dest.dump(sink);
      sink.print(",");
      m_obj.dump(sink);
      m_offset.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrLsl(dest, obj, offset));
}

void ezAsmInstruction::lsl(const ezAddress dest, const ezAddress cond,
                           const ezAddress obj, const ezAddress offset) {
  class ezInstrLslWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_obj;
    ezAddress m_offset;

  public:
    ezInstrLslWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &obj, const ezAddress &offset)
        : m_dest(dest), m_cond(cond), m_obj(obj), m_offset(offset) {}
    void process(ezStackFrame &stk) {
      stk.lsl(m_dest, m_cond, m_obj, m_offset);
    }
    void dump(ezFile &sink) {
      sink.print("lsl");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_obj.dump(sink);
      m_offset.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrLslWithCond(dest, cond, obj, offset));
}

void ezAsmInstruction::lsr(const ezAddress dest, const ezAddress obj,
                           const ezAddress offset) {
  class ezInstrLsr : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_obj;
    ezAddress m_offset;

  public:
    ezInstrLsr(const ezAddress dest, const ezAddress &obj,
               const ezAddress &offset)
        : m_dest(dest), m_obj(obj), m_offset(offset) {}
    void process(ezStackFrame &stk) { stk.lsr(m_dest, m_obj, m_offset); }
    void dump(ezFile &sink) {
      sink.print("lsr");
      m_dest.dump(sink);
      sink.print(",");
      m_obj.dump(sink);
      m_offset.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrLsr(dest, obj, offset));
}

void ezAsmInstruction::lsr(const ezAddress dest, const ezAddress cond,
                           const ezAddress obj, const ezAddress offset) {
  class ezInstrLsrWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_obj;
    ezAddress m_offset;

  public:
    ezInstrLsrWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &obj, const ezAddress &offset)
        : m_dest(dest), m_cond(cond), m_obj(obj), m_offset(offset) {}
    void process(ezStackFrame &stk) {
      stk.lsr(m_dest, m_cond, m_obj, m_offset);
    }
    void dump(ezFile &sink) {
      sink.print("lsr");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_obj.dump(sink);
      m_offset.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrLsrWithCond(dest, cond, obj, offset));
}

void ezAsmInstruction::mod(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  class ezInstrMod : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrMod(const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.mod(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("mod");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrMod(dest, lsrc, rsrc));
}

void ezAsmInstruction::mod(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  class ezInstrModWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrModWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.mod(m_dest, m_cond, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("mod");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrModWithCond(dest, cond, lsrc, rsrc));
}

void ezAsmInstruction::mul(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  class ezInstrMul : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrMul(const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.mul(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("mul");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrMul(dest, lsrc, rsrc));
}

void ezAsmInstruction::mul(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  class ezInstrMulWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrMulWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.mul(m_dest, m_cond, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("mul");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrMulWithCond(dest, cond, lsrc, rsrc));
}

void ezAsmInstruction::mv(vector<ezAddress> &dest, vector<ezAddress> &src) {
  class ezInstrMv : public ezInstruction {
  private:
    vector<ezAddress> m_dest;
    vector<ezAddress> m_src;

  public:
    ezInstrMv(vector<ezAddress> &dest, vector<ezAddress> &src)
        : m_dest(dest), m_src(src) {}
    void process(ezStackFrame &stk) { stk.mv(m_dest, m_src); }
    void dump(ezFile &sink) {
      sink.print("call");
      if(m_dest.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_dest.begin() ; it != m_dest.end() ; it++)
          (*it).dump(sink);
      sink.print(",");
      if(m_src.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_src.begin() ; it != m_src.end() ; it++)
          (*it).dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrMv(dest, src));
}

void ezAsmInstruction::mv(ezAddress &dest, ezAddress &src) {
  class ezInstrMv : public ezInstruction {
  private:
    vector<ezAddress> m_dest;
    vector<ezAddress> m_src;

  public:
    ezInstrMv(ezAddress &dest, ezAddress &src) {
      m_dest.push_back(dest);
      m_src.push_back(src);
    }
    void process(ezStackFrame &stk) { stk.mv(m_dest, m_src); }
    void dump(ezFile &sink) {
      sink.print("call");
      if(m_dest.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_dest.begin() ; it != m_dest.end() ; it++)
          (*it).dump(sink);
      sink.print(",");
      if(m_src.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_src.begin() ; it != m_src.end() ; it++)
          (*it).dump(sink);
      sink.print("\n");
    }
  };
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
  class ezInstrOr : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrOr(const ezAddress dest, const ezAddress &lsrc,
              const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.b_or(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("or");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrOr(dest, lsrc, rsrc));
}

void ezAsmInstruction::bitwise_or(const ezAddress dest, const ezAddress cond,
                                  const ezAddress &lsrc,
                                  const ezAddress &rsrc) {
  class ezInstrOrWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrOrWithCond(const ezAddress dest, const ezAddress cond,
                      const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) {
      stk.b_or(m_dest, m_cond, m_lsrc, m_rsrc);
    }
    void dump(ezFile &sink) {
      sink.print("or");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrOrWithCond(dest, cond, lsrc, rsrc));
}

void ezAsmInstruction::powv(const ezAddress dest, const ezAddress &lsrc,
                            const ezAddress &rsrc) {
  class ezInstrPow : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrPow(const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.pwr(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("pow");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrPow(dest, lsrc, rsrc));
}

void ezAsmInstruction::powv(const ezAddress dest, const ezAddress cond,
                            const ezAddress &lsrc, const ezAddress &rsrc) {
  class ezInstrPowWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrPowWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.pwr(m_dest, m_cond, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("pow");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrPowWithCond(dest, cond, lsrc, rsrc));
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
      if(m_rets.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_rets.begin() ; it != m_rets.end() ; it++)
          (*it).dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrRet(src));
}

void ezAsmInstruction::sub(const ezAddress dest, const ezAddress &lsrc,
                           const ezAddress &rsrc) {
  class ezInstrSub : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrSub(const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.sub(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("sub");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrSub(dest, lsrc, rsrc));
}

void ezAsmInstruction::sub(const ezAddress dest, const ezAddress cond,
                           const ezAddress &lsrc, const ezAddress &rsrc) {
  class ezInstrSubWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrSubWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.sub(m_dest, m_cond, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("sub");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrSubWithCond(dest, cond, lsrc, rsrc));
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
                           vector<ezAddress> &rets, const ezAddress &handle) {
  class ezInstrThd : public ezInstruction {
  private:
    ezAddress m_func;
    vector<ezAddress> m_args;
    vector<ezAddress> m_rets;
    ezAddress m_handle;

  public:
    ezInstrThd(const ezAddress &func, vector<ezAddress> &args,
               vector<ezAddress> &rets, const ezAddress &handle)
        : m_func(func), m_args(args), m_rets(rets), m_handle(handle) {}
    void process(ezStackFrame &stk) {
      stk.thd(m_func, m_args, m_rets, m_handle);
    }
    void dump(ezFile &sink) {
      sink.print("thd");
      m_func.dump(sink);
      if(m_args.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_args.begin() ; it != m_args.end() ; it++)
          (*it).dump(sink);
      sink.print(",");
      if(m_rets.empty())
        sink.print(" null");
      else
        for(vector<ezAddress>::iterator it = m_rets.begin() ; it != m_rets.end() ; it++)
          (*it).dump(sink);
      m_handle.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrThd(func, args, rets, handle));
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
  class ezInstrXor : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrXor(const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) { stk.b_xor(m_dest, m_lsrc, m_rsrc); }
    void dump(ezFile &sink) {
      sink.print("xor");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrXor(dest, lsrc, rsrc));
}

void ezAsmInstruction::bitwise_xor(const ezAddress dest, const ezAddress cond,
                                   const ezAddress &lsrc,
                                   const ezAddress &rsrc) {
  class ezInstrXorWithCond : public ezInstruction {
  private:
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

  public:
    ezInstrXorWithCond(const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}
    void process(ezStackFrame &stk) {
      stk.b_xor(m_dest, m_cond, m_lsrc, m_rsrc);
    }
    void dump(ezFile &sink) {
      sink.print("xor");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
    }
  };
  m_instruction.push_back(new ezInstrXorWithCond(dest, cond, lsrc, rsrc));
}

void ezAsmInstruction::user_command(ezInstruction *inst) {
  m_instruction.push_back(inst);
}
