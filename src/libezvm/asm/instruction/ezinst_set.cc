#include "ezinst_set.h"
#include "ezvm/ezstack.h"

ezInstrSet::ezInstrSet(
    ezALU *alu, const ezAddress &container,
    const ezAddress &member, const ezAddress val)
    : m_alu(alu), m_container(container), m_member(member), m_val(val) {
}

void ezInstrSet::process(ezStackFrame &stk) {
  ezValue *vc = (ezValue *)stk.addr2val(m_container);
  ezValue *vm = (ezValue *)stk.addr2val(m_member);
  ezValue *vv = (ezValue *)stk.addr2val(m_val);
  m_alu->set(vc, vm, vv);
}

void ezInstrSet::dump(ezFile &sink) {
  sink.print("get");
  m_container.dump(sink);
  sink.print(",");
  m_member.dump(sink);
  sink.print(",");
  m_val.dump(sink);
  sink.print("\n");
}

ezInstrSetByArray::ezInstrSetByArray(
    ezALU *alu, const ezAddress &container,
    vector<ezAddress> &member, const ezAddress val)
    : m_alu(alu), m_container(container), m_member(member), m_val(val) {}

void ezInstrSetByArray::process(ezStackFrame &stk) {
  ezValue *vc = (ezValue *)stk.addr2val(m_container);
  vector<ezObject *>vm;
  stk.addr2val(vm, m_member);
  ezValue *vv = (ezValue *)stk.addr2val(m_val);
  m_alu->set(vc, vm, vv);
}

void ezInstrSetByArray::dump(ezFile &sink) {
  sink.print("get");
  m_container.dump(sink);
  sink.print(",");
  for (vector<ezAddress>::iterator it = m_member.begin(); it != m_member.end();
       it++)
    (*it).dump(sink);
  sink.print(",");
  m_val.dump(sink);
  sink.print("\n");
}


