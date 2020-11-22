#include "ezinst_mv.h"
#include "ezvm/ezlog.h"
#include "ezvm/ezstack.h"

ezInstrMv::ezInstrMv(vector<ezAddress> &dest, vector<ezAddress> &src)
    : m_dest(dest), m_src(src) {
}

ezInstrMv::ezInstrMv(ezAddress &dest, ezAddress &src) {
  m_dest.push_back(dest);
  m_src.push_back(src);
}

void ezInstrMv::process(ezStackFrame &stk) {
  size_t cnt = m_dest.size();
  vector<ezObject *> q;
  stk.addr2val(q, m_src);
  stk.val2addr(m_dest, q);
  size_t i = q.size();
  while (i < cnt) {
    EZ_INFO("%d:%d <== 0x%x", m_dest[i].segment, m_dest[i].offset,
            ezNull::instance());
    stk.val2addr(m_dest[i++], ezNull::instance());
  }
}

void ezInstrMv::dump(ezFile &sink) {
  sink.print("mv,");
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

ezInstrMvWithOp::ezInstrMvWithOp(ezALU* alu, vector<ezAddress> &dest, vector<ezAddress> &src, char op, function<ezValue *(ezALU *, ezValue *, ezValue *)> func)
    : m_alu(alu), m_dest(dest), m_src(src), m_op(op), m_func(func) {}

ezInstrMvWithOp::ezInstrMvWithOp(ezALU* alu, ezAddress &dest, ezAddress &src, char op, function<ezValue *(ezALU *, ezValue *, ezValue *)> func) : m_alu(alu), m_func(func), m_op(op) {
  m_dest.push_back(dest);
  m_src.push_back(src);
}

void ezInstrMvWithOp::process(ezStackFrame &stk) {
  size_t cnt = m_dest.size();
  ezValue *v = NULL, *l = NULL, *r = NULL;
  size_t i = 0;
  vector<ezObject *> q;
  stk.addr2val(q, m_src);
  for (i = 0; i < cnt && i < q.size(); i++) {
    EZ_INFO("%d:%d <%c= 0x%x", m_dest[i].segment, m_dest[i].offset, q[i]);
    v = ezValue::cast(stk.addr2val(m_dest[i]));
    l = ezValue::cast(q[i]);
    r = m_func(m_alu, v, l);
    stk.val2addr(m_dest[i], r);
  }
  while (i < cnt) {
    EZ_INFO("%d:%d <== 0x%x", m_dest[i].segment, m_dest[i].offset,
            ezNull::instance());
    stk.val2addr(m_dest[i++], ezNull::instance());
  }
}

void ezInstrMvWithOp::dump(ezFile &sink) {
  sink.print("mvo,%c,", m_op);
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
