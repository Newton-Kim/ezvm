#pragma once
#include "ezvm/ezinstruction.h"

class ezInstrMv : public ezInstruction {
private:
  vector<ezAddress> m_dest;
  vector<ezAddress> m_src;

public:
  ezInstrMv(vector<ezAddress> &dest, vector<ezAddress> &src);
  ezInstrMv(ezAddress &dest, ezAddress &src);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
