#pragma once
#include "ezvm/ezinstruction.h"

class ezInstrCallVoid : public ezInstruction {
private:
  ezAddress m_func;
  vector<ezAddress> m_args;

public:
  ezInstrCallVoid(const ezAddress &func, vector<ezAddress> &args);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

class ezInstrCallFn : public ezInstruction {
private:
  ezAddress m_func;
  vector<ezAddress> m_args;
  ezAddress m_ret;

public:
  ezInstrCallFn(const ezAddress &func, vector<ezAddress> &args, ezAddress &ret);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
