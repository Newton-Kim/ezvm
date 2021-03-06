#include "ezinst_get.h"
#include "ezvm/ezstack.h"

ezInstrGet::ezInstrGet(const ezAddress dest, const ezAddress &container,
                       const ezAddress &member)
    : m_dest(dest), m_container(container), m_member(member) {}

void ezInstrGet::process(ezStackFrame &stk) {
  ezValue *vc = (ezValue *)stk.addr2val(m_container);
  ezValue *vm = (ezValue *)stk.addr2val(m_member);
  ezObject *rst = vc->get(vm);
  stk.val2addr(m_dest, rst);
}

void ezInstrGet::dump(ezFile &sink) {
  sink.print("get");
  m_dest.dump(sink);
  sink.print(",");
  m_container.dump(sink);
  sink.print(",");
  m_member.dump(sink);
  sink.print("\n");
}

ezInstrGetByArray::ezInstrGetByArray(const ezAddress dest,
                                     const ezAddress &container,
                                     vector<ezAddress> &member)
    : m_dest(dest), m_container(container), m_member(member) {}

void ezInstrGetByArray::process(ezStackFrame &stk) {
  ezValue *vc = ezValue::cast(stk.addr2val(m_container));
  ezValue *vme = NULL;
  vector<ezValue *> vm;
  for (vector<ezAddress>::iterator it = m_member.begin(); it != m_member.end();
       it++) {
    vme = ezValue::cast(stk.addr2val(*it));
    vm.push_back(vme);
  }
  ezObject *rst = vc->get(vm);
  stk.val2addr(m_dest, rst);
}

void ezInstrGetByArray::dump(ezFile &sink) {
  sink.print("gta");
  m_dest.dump(sink);
  sink.print(",");
  m_container.dump(sink);
  sink.print(",");
  for (vector<ezAddress>::iterator it = m_member.begin(); it != m_member.end();
       it++)
    (*it).dump(sink);
  sink.print("\n");
}
