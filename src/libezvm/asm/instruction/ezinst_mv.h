#pragma once
#include "ezvm/ezalu.h"
#include "ezvm/ezinstruction.h"

class ezInstrMv : public ezInstruction {
private:
  ezALU* m_alu;
  vector<ezAddress> m_dest;
  vector<ezAddress> m_src;
  char m_op;

public:
  ezInstrMv(ezALU* alu, vector<ezAddress> &dest, vector<ezAddress> &src, char op);
  ezInstrMv(ezALU* alu, ezAddress &dest, ezAddress &src, char op);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
