#include "ezinst_mv.h"
#include "ezvm/ezlog.h"
#include "ezvm/ezstack.h"

ezInstrMv::ezInstrMv(ezALU* alu, vector<ezAddress> &dest, vector<ezAddress> &src, char op)
    : m_alu(alu), m_dest(dest), m_src(src), m_op(op) {}

ezInstrMv::ezInstrMv(ezALU* alu, ezAddress &dest, ezAddress &src, char op) : m_alu(alu), m_op(op) {
  m_dest.push_back(dest);
  m_src.push_back(src);
}

void ezInstrMv::process(ezStackFrame &stk) {
  size_t cnt = m_dest.size();
  ezValue *v = NULL, *r = NULL;
  size_t i = 0;
  vector<ezObject *> q;
  stk.addr2val(q, m_src);
  for (i = 0; i < cnt && i < q.size(); i++) {
    EZ_INFO("%d:%d <%c= 0x%x", m_dest[i].segment, m_dest[i].offset, q[i]);
    if(m_op == '=') {
      stk.val2addr(m_dest[i], q[i]);
    } else {
      v = (ezValue*) stk.addr2val(m_dest[i]);
      switch (m_op) {
      case '+':
        r = m_alu->add(v, (ezValue*)q[i]);
        break;
      case '-':
        r = m_alu->subtract(v, (ezValue*)q[i]);
        break;
      case '*':
        r = m_alu->multiply(v, (ezValue*)q[i]);
        break;
      case '/':
        r = m_alu->divide(v, (ezValue*)q[i]);
        break;
      case '%':
        r = m_alu->modulate(v, (ezValue*)q[i]);
        break;
      case '&':
        r = m_alu->bitwise_and(v, (ezValue*)q[i]);
        break;
      case '>':
        r = v->lsr((ezValue*)q[i]);
        break;
      case '<':
        r = v->lsl((ezValue*)q[i]);
        break;
      case '|':
        r = m_alu->bitwise_or(v, (ezValue*)q[i]);
        break;
      case '^':
        r = m_alu->bitwise_xor(v, (ezValue*)q[i]);
        break;
      default:
        throw runtime_error("invalid operator");
        break;
      }
      stk.val2addr(m_dest[i], r);
    }
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
