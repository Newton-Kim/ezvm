#include "ezvm/ezinstruction.h"
#include "ezvm/ezalu.h"

class ezInstrAdd : public ezInstruction {
private:
    ezALU *m_alu;
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

public:
  ezInstrAdd(ezALU *alu, const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

class ezInstrAddWithCond : public ezInstruction {
private:
    ezALU *m_alu;
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

public:
  ezInstrAddWithCond(ezALU *alu, const ezAddress dest, const ezAddress cond,
                     const ezAddress &lsrc, const ezAddress &rsrc);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
