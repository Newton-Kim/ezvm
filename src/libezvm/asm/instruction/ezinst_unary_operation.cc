#include "ezinst_unary_operation.h"
#include "ezvm/ezlog.h"
#include "ezvm/ezstack.h"

ezInstrUnaryOperation::ezInstrUnaryOperation(
    const ezAddress dest, const ezAddress &src, const string name,
    function<ezValue *(ezValue *)> func)
    : m_func(func), m_name(name), m_dest(dest), m_src(src) {}

void ezInstrUnaryOperation::process(ezStackFrame &stk) {
  ezValue *v = ezValue::cast(stk.addr2val(m_src));
  ezValue *rst = m_func(v);
  stk.val2addr(m_dest, rst);
}

void ezInstrUnaryOperation::dump(ezFile &sink) {
  sink.print(m_name.c_str());
  m_dest.dump(sink);
  sink.print(",");
  m_src.dump(sink);
  sink.print("\n");
}

ezInstrUnaryOperationWithCond::ezInstrUnaryOperationWithCond(
    const ezAddress dest, const ezAddress cond, const ezAddress &src,
    const string name, function<ezValue *(ezValue *)> func)
    : m_func(func), m_name(name), m_dest(dest), m_cond(cond), m_src(src) {}

void ezInstrUnaryOperationWithCond::process(ezStackFrame &stk) {
  ezValue *v = ezValue::cast(stk.addr2val(m_src));
  ezValue *rst = m_func(v);
  stk.val2addr(m_dest, rst);
  stk.val2addr(m_cond, rst->condition());
}

void ezInstrUnaryOperationWithCond::dump(ezFile &sink) {
  sink.print(m_name.c_str());
  m_dest.dump(sink);
  m_cond.dump(sink);
  sink.print(",");
  m_src.dump(sink);
  sink.print("\n");
}
