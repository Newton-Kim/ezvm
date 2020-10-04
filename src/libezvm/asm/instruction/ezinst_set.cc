#include "ezinst_set.h"
#include "ezvm/ezstack.h"

ezInstrSet::ezInstrSet(const ezAddress &container, const ezAddress &member,
                       const ezAddress val)
    : m_container(container), m_member(member), m_val(val) {}

void ezInstrSet::process(ezStackFrame &stk) {
  ezValue *vc = ezValue::cast(stk.addr2val(m_container));
  ezValue *vm = ezValue::cast(stk.addr2val(m_member));
  ezObject *vv = stk.addr2val(m_val);
  vc->set(vm, vv);
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

ezInstrSetByArray::ezInstrSetByArray(const ezAddress &container,
                                     vector<ezAddress> &member,
                                     const ezAddress val)
    : m_container(container), m_member(member), m_val(val) {}

void ezInstrSetByArray::process(ezStackFrame &stk) {
  ezValue *vc = ezValue::cast(stk.addr2val(m_container));
  vector<ezValue *> vm;
  ezValue *vme = NULL;
  for (vector<ezAddress>::iterator it = m_member.begin(); it != m_member.end();
       it++) {
    vme = ezValue::cast(stk.addr2val(*it));
    vm.push_back(vme);
  }
  ezObject *vv = stk.addr2val(m_val);
  vc->set(vm, vv);
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
