#pragma once
#include "ezvm/ezalu.h"
#include "ezvm/ezinstruction.h"

class ezInstrCoerc : public ezInstruction {
private:
  ezALU * m_alu;
  ezAddress m_dest;
  ezAddress m_src;
  int m_type;

public:
  ezInstrCoerc(ezALU *alu, const ezAddress dest, const ezAddress src,
             const int type);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

class ezInstrCoercWithCond : public ezInstruction {
private:
  ezALU * m_alu;
  ezAddress m_dest;
  ezAddress m_cond;
  ezAddress m_src;
  int m_type;

public:
  ezInstrCoercWithCond(ezALU *alu, const ezAddress dest, const ezAddress cond,
                    const ezAddress src, const int type);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
