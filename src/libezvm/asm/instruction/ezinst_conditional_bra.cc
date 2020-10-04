#include "ezinst_conditional_bra.h"
#include "ezvm/ezlog.h"
#include "ezvm/ezstack.h"

ezInstrConditionalBra::ezInstrConditionalBra(const ezAddress &cond,
                                             const size_t index,
                                             const string name,
                                             function<bool(ezCondition *)> func)
    : m_func(func), m_name(name), m_cond(cond), m_index(index) {}

void ezInstrConditionalBra::process(ezStackFrame &stk) {
  ezCondition *vcond = ezCondition::cast(stk.addr2val(m_cond));
  if (m_func(vcond)) stk.bra(m_index);
}

void ezInstrConditionalBra::dump(ezFile &sink) {
  sink.print(m_name.c_str());
  m_cond.dump(sink);
  sink.print(" %lu\n", m_index);
}
