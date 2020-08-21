#pragma once
#include "ezvm/ezalu.h"
#include "ezvm/ezinstruction.h"

class ezInstrCmp : public ezInstruction {
private:
  ezALU *m_alu;
  string m_name;
  ezAddress m_dest;
  ezAddress m_lsrc;
  ezAddress m_rsrc;

public:
  ezInstrCmp(ezALU *alu, const ezAddress dest, const ezAddress &lsrc,
             const ezAddress &rsrc);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
