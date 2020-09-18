#pragma once
#include "ezvm/ezalu.h"
#include "ezvm/ezinstruction.h"

#include <functional>

using std::function;

class ezInstrUnaryOperation : public ezInstruction {
private:
  function<ezValue *(ezValue *)> m_func;
  string m_name;
  ezAddress m_dest;
  ezAddress m_src;

public:
  ezInstrUnaryOperation(const ezAddress dest, const ezAddress &src,
                        const string name, function<ezValue *(ezValue *)> func);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

class ezInstrUnaryOperationWithCond : public ezInstruction {
private:
  function<ezValue *(ezValue *)> m_func;
  string m_name;
  ezAddress m_dest;
  ezAddress m_cond;
  ezAddress m_src;

public:
  ezInstrUnaryOperationWithCond(const ezAddress dest, const ezAddress cond,
                                const ezAddress &src, const string name,
                                function<ezValue *(ezValue *)> func);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
