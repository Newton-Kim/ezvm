#include "ezinst_coerc.h"
#include "ezvm/ezstack.h"

ezInstrCoerc::ezInstrCoerc(ezALU* alu, const ezAddress dest, const ezAddress src,
                       const int type)
    : m_dest(dest), m_src(src), m_type(type) {}

void ezInstrCoerc::process(ezStackFrame &stk) {
  ezValue *src = (ezValue *)stk.addr2val(m_src);
  ezValue *rst = m_alu->coerc(src, m_type);
  stk.val2addr(m_dest, rst);
}

void ezInstrCoerc::dump(ezFile &sink) {
  sink.print("coc");
  m_dest.dump(sink);
  sink.print(",");
  m_src.dump(sink);
  sink.print(", %lu", m_type);
  sink.print("\n");
}

ezInstrCoercWithCond::ezInstrCoercWithCond(ezALU* alu, const ezAddress dest,
		const ezAddress cond, const ezAddress src, const int type)
    : m_dest(dest), m_cond(cond), m_src(src), m_type(type) {}

void ezInstrCoercWithCond::process(ezStackFrame &stk) {
  ezValue *src = (ezValue *)stk.addr2val(m_src);
  ezValue *rst = m_alu->coerc(src, m_type);
  stk.val2addr(m_dest, rst);
  stk.val2addr(m_cond, rst->condition());
}

void ezInstrCoercWithCond::dump(ezFile &sink) {
  sink.print("coc");
  m_dest.dump(sink);
  m_cond.dump(sink);
  sink.print(",");
  m_src.dump(sink);
  sink.print(", %lu", m_type);
  sink.print("\n");
}
