#pragma once
#include "ezvm/ezalu.h"
#include "ezvm/ezinstruction.h"

class ezInstrDup : public ezInstruction {
private:
  ezAddress m_dest;
  ezAddress m_src;

public:
  ezInstrDup(const ezAddress dest, const ezAddress &src);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
