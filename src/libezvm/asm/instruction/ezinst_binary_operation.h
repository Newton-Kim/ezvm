#include "ezvm/ezinstruction.h"
#include "ezvm/ezalu.h"

#include <functional>

using std::function;

class ezInstrBinaryOperation : public ezInstruction {
private:
    ezALU *m_alu;
    function<ezValue *(ezALU *, ezValue *, ezValue *)> m_func;
    string m_name;
    ezAddress m_dest;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

public:
  ezInstrBinaryOperation(ezALU *alu, const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc, const string name,
	       function<ezValue *(ezALU *, ezValue *, ezValue *)> func);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

class ezInstrBinaryOperationWithCond : public ezInstruction {
private:
    ezALU *m_alu;
    function<ezValue *(ezALU *, ezValue *, ezValue *)> m_func;
    string m_name;
    ezAddress m_dest;
    ezAddress m_cond;
    ezAddress m_lsrc;
    ezAddress m_rsrc;

public:
  ezInstrBinaryOperationWithCond(ezALU *alu, const ezAddress dest, const ezAddress cond,
                     const ezAddress &lsrc, const ezAddress &rsrc, const string name,
	             function<ezValue *(ezALU *, ezValue *, ezValue *)> func);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
