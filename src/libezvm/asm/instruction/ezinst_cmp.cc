#include "ezinst_cmp.h"
#include "ezvm/ezlog.h"
#include "ezvm/ezstack.h"

ezInstrCmp::ezInstrCmp(ezALU *alu, const ezAddress dest, const ezAddress &lsrc,
                       const ezAddress &rsrc)
    : m_alu(alu), m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}

void ezInstrCmp::process(ezStackFrame &stk) {
  ezValue *vl = (ezValue *)stk.addr2val(m_lsrc);
  ezValue *vr = (ezValue *)stk.addr2val(m_rsrc);
  ezObject *rst = m_alu->compare(vl, vr);
  stk.val2addr(m_dest, rst);
}

void ezInstrCmp::dump(ezFile &sink) {
  sink.print("cmp");
  m_dest.dump(sink);
  sink.print(",");
  m_lsrc.dump(sink);
  m_rsrc.dump(sink);
  sink.print("\n");
}
