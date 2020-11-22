#pragma once
#include "ezvm/ezalu.h"
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

class ezInstrMvWithOp : public ezInstruction {
private:
  ezALU* m_alu;
  vector<ezAddress> m_dest;
  vector<ezAddress> m_src;
  char m_op;
  function<ezValue *(ezALU *, ezValue *, ezValue *)> m_func;

public:
  ezInstrMvWithOp(ezALU* alu, vector<ezAddress> &dest, vector<ezAddress> &src, char op, function<ezValue *(ezALU *, ezValue *, ezValue *)> func);
  ezInstrMvWithOp(ezALU* alu, ezAddress &dest, ezAddress &src, char op, function<ezValue *(ezALU *, ezValue *, ezValue *)> func);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
