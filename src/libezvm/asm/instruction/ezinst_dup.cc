#include "ezinst_dup.h"
#include "ezvm/ezlog.h"
#include "ezvm/ezstack.h"

ezInstrDup::ezInstrDup(const ezAddress dest, const ezAddress &src)
    : m_dest(dest), m_src(src) {}

void ezInstrDup::process(ezStackFrame &stk) {
  ezValue *v = ezValue::cast(stk.addr2val(m_src));
  ezObject *rst = v->duplicate();
  stk.val2addr(m_dest, rst);
}

void ezInstrDup::dump(ezFile &sink) {
  sink.print("dup");
  m_dest.dump(sink);
  sink.print(",");
  m_src.dump(sink);
  sink.print("\n");
}
