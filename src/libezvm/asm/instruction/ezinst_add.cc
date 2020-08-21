#include "ezinst_add.h"
#include "ezvm/ezstack.h"
#include "ezvm/ezlog.h"

ezInstrAdd::ezInstrAdd(ezALU *alu, const ezAddress dest, const ezAddress &lsrc,
               const ezAddress &rsrc)
        : m_alu(alu), m_dest(dest), m_lsrc(lsrc), m_rsrc(rsrc) {}

void ezInstrAdd::process(ezStackFrame &stk) {
  ezValue *vl = (ezValue *)stk.addr2val(m_lsrc);
  ezValue *vr = (ezValue *)stk.addr2val(m_rsrc);
  ezValue *rst = m_alu->add(vl, vr);
  stk.val2addr(m_dest, rst);
}

void ezInstrAdd::dump(ezFile &sink) {
      sink.print("add");
      m_dest.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
}

ezInstrAddWithCond::ezInstrAddWithCond(ezALU *alu, const ezAddress dest, const ezAddress cond,
                       const ezAddress &lsrc, const ezAddress &rsrc)
        : m_alu(alu), m_dest(dest), m_cond(cond), m_lsrc(lsrc), m_rsrc(rsrc) {}

void ezInstrAddWithCond::process(ezStackFrame &stk) {
  ezValue *vl = (ezValue *)stk.addr2val(m_lsrc);
  ezValue *vr = (ezValue *)stk.addr2val(m_rsrc);
  ezValue *rst = m_alu->add(vl, vr);
  stk.val2addr(m_dest, rst);
  stk.val2addr(m_cond, m_alu->condition(rst));
}

void ezInstrAddWithCond::dump(ezFile &sink) {
      sink.print("add");
      m_dest.dump(sink);
      m_cond.dump(sink);
      sink.print(",");
      m_lsrc.dump(sink);
      m_rsrc.dump(sink);
      sink.print("\n");
}

