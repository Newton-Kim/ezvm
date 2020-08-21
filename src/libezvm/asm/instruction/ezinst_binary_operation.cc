#include "ezinst_binary_operation.h"
#include "ezvm/ezstack.h"
#include "ezvm/ezlog.h"

ezInstrBinaryOperation::ezInstrBinaryOperation(ezALU *alu, const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc, const string name,
	       function<ezValue *(ezALU *, ezValue *, ezValue *)> func)
        : m_alu(alu), m_func(func), m_name(name), m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}

void ezInstrBinaryOperation::process(ezStackFrame &stk) {
  ezValue *vl = (ezValue *)stk.addr2val(m_lsrc);
  ezValue *vr = (ezValue *)stk.addr2val(m_rsrc);
  ezValue *rst = m_func(m_alu, vl, vr);
  stk.val2addr(m_dest, rst);
}

void ezInstrBinaryOperation::dump(ezFile &sink) {
      sink.print(m_name.c_str());
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
}

ezInstrBinaryOperationWithCond::ezInstrBinaryOperationWithCond(ezALU *alu, const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc, const string name,
	               function<ezValue *(ezALU *, ezValue *, ezValue *)> func)
        : m_alu(alu), m_func(func), m_name(name), m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}

void ezInstrBinaryOperationWithCond::process(ezStackFrame &stk) {
  ezValue *vl = (ezValue *)stk.addr2val(m_lsrc);
  ezValue *vr = (ezValue *)stk.addr2val(m_rsrc);
  ezValue *rst = m_func(m_alu, vl, vr);
  stk.val2addr(m_dest, rst);
  stk.val2addr(m_cond, m_alu->condition(rst));
}

void ezInstrBinaryOperationWithCond::dump(ezFile &sink) {
      sink.print(m_name.c_str());
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
}

