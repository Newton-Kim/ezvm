#include "ezvm/ezalu.h"
#include "ezvm/ezinstruction.h"

#include <functional>

using std::function;

class ezInstrUnaryOperation : public ezInstruction {
private:
  ezALU *m_alu;
  function<ezValue *(ezALU *, ezValue *)> m_func;
  string m_name;
  ezAddress m_dest;
  ezAddress m_src;

public:
  ezInstrUnaryOperation(
      ezALU *alu, const ezAddress dest, const ezAddress &src,
      const string name, function<ezValue *(ezALU *, ezValue *)> func);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

class ezInstrUnaryOperationWithCond : public ezInstruction {
private:
  ezALU *m_alu;
  function<ezValue *(ezALU *, ezValue *)> m_func;
  string m_name;
  ezAddress m_dest;
  ezAddress m_cond;
  ezAddress m_src;

public:
  ezInstrUnaryOperationWithCond(
      ezALU *alu, const ezAddress dest, const ezAddress cond,
      const ezAddress &src, const string name,
      function<ezValue *(ezALU *, ezValue *)> func);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
