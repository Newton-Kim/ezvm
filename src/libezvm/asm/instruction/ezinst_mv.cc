#include "ezinst_mv.h"
#include "ezvm/ezstack.h"
#include "ezvm/ezlog.h"

ezInstrMv::ezInstrMv(vector<ezAddress> &dest, vector<ezAddress> &src)
    : m_dest(dest), m_src(src) {}

ezInstrMv::ezInstrMv(ezAddress &dest, ezAddress &src) {
  m_dest.push_back(dest);
  m_src.push_back(src);
}

void ezInstrMv::process(ezStackFrame &stk) {
  size_t cnt = m_dest.size();
  ezObject *v = NULL;
  size_t i = 0;
  vector<ezObject *> q;
  stk.addr2val(q, m_src);
  for (i = 0; i < cnt && i < q.size(); i++) {
    EZ_INFO("%d:%d <= 0x%x", m_dest[i].segment, m_dest[i].offset, q[i]);
    stk.val2addr(m_dest[i], q[i]);
  }
  while (i < cnt) {
    EZ_INFO("%d:%d <== 0x%x", m_dest[i].segment, m_dest[i].offset,
            ezNull::instance());
    stk.val2addr(m_dest[i++], ezNull::instance());
  }
}

void ezInstrMv::dump(ezFile &sink) {
  sink.print("mv");
  if (m_dest.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = m_dest.begin(); it != m_dest.end();
         it++)
      (*it).dump(sink);
  sink.print(",");
  if (m_src.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = m_src.begin(); it != m_src.end();
         it++)
      (*it).dump(sink);
  sink.print("\n");
}
