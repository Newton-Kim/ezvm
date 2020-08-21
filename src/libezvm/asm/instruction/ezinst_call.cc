#include "ezinst_call.h"
#include "ezvm/ezobject.h"
#include "ezvm/ezstack.h"

ezInstrCallFn::ezInstrCallFn(const ezAddress &func, vector<ezAddress> &args,
                             ezAddress &ret)
    : m_func(func), m_args(args), m_ret(ret) {}

void ezInstrCallFn::process(ezStackFrame &stk) {
  ezObject *proc = stk.addr2val(m_func);
  vector<ezObject *> vargs;
  stk.addr2val(vargs, m_args);
  ezObject *vret = NULL;
  switch (proc->type) {
  case EZ_OBJECT_TYPE_USER_DEFINED_FUNCTION:
    vret = ((ezUserDefinedFunction *)proc)->run(vargs);
    // TODO:if null, use ezNull
    if (vret)
      stk.val2addr(m_ret, vret);
    break;
  case EZ_OBJECT_TYPE_FUNCTION:
    stk.call((ezFunction *)proc, vargs, m_ret);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
}

void ezInstrCallFn::dump(ezFile &sink) {
  sink.print("call");
  m_func.dump(sink);
  sink.print(",");
  if (m_args.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = m_args.begin(); it != m_args.end();
         it++)
      (*it).dump(sink);
  sink.print(",");
  m_ret.dump(sink);
  sink.print("\n");
}

ezInstrCallVoid::ezInstrCallVoid(const ezAddress &func, vector<ezAddress> &args)
    : m_func(func), m_args(args) {}

void ezInstrCallVoid::process(ezStackFrame &stk) {
  ezObject *proc = stk.addr2val(m_func);
  vector<ezObject *> vargs;
  stk.addr2val(vargs, m_args);
  switch (proc->type) {
  case EZ_OBJECT_TYPE_USER_DEFINED_FUNCTION:
    ((ezUserDefinedFunction *)proc)->run(vargs);
    break;
  case EZ_OBJECT_TYPE_FUNCTION:
    stk.call((ezFunction *)proc, vargs);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
}

void ezInstrCallVoid::dump(ezFile &sink) {
  sink.print("call");
  m_func.dump(sink);
  sink.print(",");
  if (m_args.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = m_args.begin(); it != m_args.end();
         it++)
      (*it).dump(sink);
  sink.print(", null");
  sink.print("\n");
}
