/* Copyright (C) 2015 Newton Kim
*
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditiong:
*
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/
#include "ezvm/ezinstruction.h"
#include "ezvm/ezmemory.h"
#include "ezvm/ezstack.h"
#include <iostream>
#include <stdexcept>

ezStackFrame::ezStackFrame(ezFunction *crsl, vector<ezValue *> &args,
                           vector<ezAddress> &rets,
                           ezStackFrameCallback *callback)
    : m_pc(0), m_local(m_carousel->local_memory()),
      m_scope(m_carousel->scope_memory()), m_carousel(crsl),
      m_callback(callback) {
  if (!m_callback)
    throw runtime_error("callback is null.");
  size_t min_args = (crsl->nargs > args.size()) ? args.size() : crsl->nargs;
  for (size_t i = 0; i < min_args; i++)
    (*m_local)[i] = args[i];
  for (size_t i = 0; i < rets.size(); i++)
    m_return_dest.push_back(rets[i]);
  m_memory.push_back(&(ezMemory::instance().globals().to_vector()));
  m_memory.push_back(&ezMemory::instance().constants());
  m_memory.push_back(m_local);
  m_memory.push_back(m_scope);
}

ezStackFrame::~ezStackFrame() {
  if (!m_carousel->is_local_scoped())
    delete m_local;
}

void ezStackFrame::addr2val(vector<ezValue *> &vals, vector<ezAddress> &addr) {
  for (size_t i = 0; i < addr.size(); i++)
    vals.push_back(addr2val(addr[i]));
}

ezValue *ezStackFrame::addr2val(ezAddress addr) {
  return m_memory[addr.segment]->operator[](addr.offset);
}

void ezStackFrame::val2addr(vector<ezAddress> &addr, vector<ezValue *> &vals) {
  size_t addr_sz = addr.size(), vals_sz = vals.size();
  size_t gcv = (addr_sz > vals_sz) ? vals_sz : addr_sz;
  for (size_t i = 0; i < gcv; i++) {
    ezValue *v = vals[i];
    val2addr(addr[i], v);
  }
  if (addr_sz > vals_sz) {
    for (size_t i = gcv; i < addr_sz; i++)
      val2addr(addr[i], ezNull::instance());
  } else if (addr_sz < vals_sz) {
    for (size_t i = gcv; i < vals_sz; i++)
      delete vals[i];
  }
}

void ezStackFrame::val2addr(ezAddress addr, ezValue *v) {
  m_memory[addr.segment]->operator[](addr.offset) = v;
  ezGC::instance().add((ezGCObject *)v);
}

void ezStackFrame::operate(ezBinaryOperation op, ezAddress &dest,
                           ezAddress &src1, ezAddress &src2) {
  ezValue *rst = NULL;
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(src1);
  vr = addr2val(src2);
  rst = vl->operate(op, vr);
  val2addr(dest, rst);
}

void ezStackFrame::operate(ezBinaryOperation op, ezAddress &dest,
                           ezAddress &cond, ezAddress &src1, ezAddress &src2) {
  ezValue *rst = NULL;
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(src1);
  vr = addr2val(src2);
  rst = vl->operate(op, vr);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::conditional_bra(ezAddress &cond, size_t index,
                                   function<bool(ezCondition *)> func) {
  ezValue *vcond = addr2val(cond);
  if (vcond->type != EZ_VALUE_TYPE_CONDITION)
    throw runtime_error("The operation doesn't see condition");
  if (func((ezCondition *)vcond))
    bra(index);
}

void ezStackFrame::beq(ezAddress &cond, size_t index) {
  conditional_bra(cond, index, [](ezCondition *cond) { return cond->zero; });
}

void ezStackFrame::bge(ezAddress &cond, size_t index) {
  conditional_bra(cond, index, [](ezCondition *cond) {
    return (cond->zero || !cond->negative);
  });
}

void ezStackFrame::blt(ezAddress &cond, size_t index) {
  conditional_bra(cond, index,
                  [](ezCondition *cond) { return cond->negative; });
}

void ezStackFrame::bne(ezAddress &cond, size_t index) {
  conditional_bra(cond, index, [](ezCondition *cond) { return !cond->zero; });
}

void ezStackFrame::bra(size_t index) {
  if (m_carousel->jmptbl.size() <= index)
    throw runtime_error("tried to jump to an invalid label");
  m_pc = m_carousel->jmptbl[index];
}

void ezStackFrame::test_equality(ezAddress &rst, ezAddress &lsrc,
                                 ezAddress &rsrc,
                                 function<ezValue *(ezCondition *)> func) {
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(lsrc);
  vr = addr2val(rsrc);
  ezValue *cond = vl->operate(EZ_BINARY_OPERATION_COMPARISON, vr);
  val2addr(rst, func((ezCondition *)cond));
  delete cond;
}

void ezStackFrame::test_equality(ezAddress &rst, ezAddress &cond,
                                 ezAddress &lsrc, ezAddress &rsrc,
                                 function<ezValue *(ezCondition *)> func) {
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(lsrc);
  vr = addr2val(rsrc);
  ezValue *vcond = vl->operate(EZ_BINARY_OPERATION_COMPARISON, vr);
  val2addr(rst, func((ezCondition *)vcond));
  val2addr(cond, vcond);
}

void ezStackFrame::teq(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->zero) ? new ezBool(true) : new ezBool(false);
  });
}

void ezStackFrame::teq(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  test_equality(dest, cond, src1, src2, [](ezCondition *cond) {
    return (cond->zero) ? new ezBool(true) : new ezBool(false);
  });
}

void ezStackFrame::tge(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->zero || !cond->negative) ? new ezBool(true)
                                           : new ezBool(false);
  });
}

void ezStackFrame::tge(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  test_equality(dest, cond, src1, src2, [](ezCondition *cond) {
    return (cond->zero || !cond->negative) ? new ezBool(true)
                                           : new ezBool(false);
  });
}

void ezStackFrame::tlt(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->negative) ? new ezBool(true) : new ezBool(false);
  });
}

void ezStackFrame::tlt(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->negative) ? new ezBool(true) : new ezBool(false);
  });
}

void ezStackFrame::tne(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (!cond->zero) ? new ezBool(true) : new ezBool(false);
  });
}

void ezStackFrame::tne(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (!cond->zero) ? new ezBool(true) : new ezBool(false);
  });
}

void ezStackFrame::operate(ezUnaryOperation op, ezAddress &dest,
                           ezAddress &src) {
  ezValue *v = NULL, *rst = NULL;
  v = addr2val(src);
  rst = v->operate(op);
  val2addr(dest, rst);
}

void ezStackFrame::operate(ezUnaryOperation op, ezAddress &dest,
                           ezAddress &cond, ezAddress &src) {
  ezValue *v = NULL, *rst = NULL;
  v = addr2val(src);
  rst = v->operate(op);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::fgc(void) { ezGC::instance().force(); }

void ezStackFrame::ret(vector<ezAddress> &srcs) {
  ezAddress dest, addr, cond;
  ezValue *v = NULL;
  for (size_t i = 0; i < srcs.size(); i++) {
    v = addr2val(srcs[i]);
    m_returns.push_back(v);
  }
  m_callback->end(m_return_dest, m_returns);
}

void ezStackFrame::mv(vector<ezAddress> &dests, vector<ezAddress> &srcs) {
  size_t cnt = (dests.size() > srcs.size()) ? srcs.size() : dests.size();
  ezValue *v = NULL;
  size_t i = 0;
  vector<ezValue *> q;
  addr2val(q, srcs);
  for (i = 0; i < cnt; i++) {
    val2addr(dests[i], q[i]);
  }
  if (dests.size() > srcs.size()) {
    for (i = cnt; i < dests.size(); i++) {
      val2addr(dests[i], ezNull::instance());
    }
  }
}

void ezStackFrame::call(ezAddress &func, vector<ezAddress> &args,
                        vector<ezAddress> &rets) {
  ezValue *proc = addr2val(func);
  vector<ezValue *> vargs;
  addr2val(vargs, args);
  switch (proc->type) {
  case EZ_VALUE_TYPE_USER_DEFINED_FUNCTION:
    call((ezUserDefinedFunction *)proc, vargs, rets);
    break;
  case EZ_VALUE_TYPE_FUNCTION:
    call((ezFunction *)proc, vargs, rets);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
}

void ezStackFrame::call(ezFunction *func, vector<ezValue *> &args,
                        vector<ezAddress> &rets) {
  ezGC::instance().pause();
  ezStackFrame *callee = new ezStackFrame(func, args, rets, m_callback);
  m_callback->call(callee);
  ezGC::instance().resume();
}

void ezStackFrame::call(ezUserDefinedFunction *func, vector<ezValue *> &args,
                        vector<ezAddress> &rets) {
  vector<ezValue *> vrets;
  func->run(args, vrets);
  if (rets.size())
    val2addr(rets, vrets);
}

void ezStackFrame::thd(ezAddress &func, vector<ezAddress> &args,
                       vector<ezAddress> &rets, ezAddress &handle) {
  vector<ezValue *> vargs;
  addr2val(vargs, args);
  size_t hthd = m_callback->thd(func, vargs, rets, this);
  val2addr(handle, new ezInteger(hthd));
}

void ezStackFrame::wait(ezAddress &handle) {
  ezValue *v = addr2val(handle);
  if (v->type != EZ_VALUE_TYPE_INTEGER)
    throw runtime_error("invalid handle");
  m_callback->wait(((ezInteger *)v)->value);
}

void ezStackFrame::update(vector<ezAddress> &dests, vector<ezValue *> &vals) {
  val2addr(dests, vals);
}

void ezStackFrame::step(void) {
  if (m_pc >= m_carousel->instruction.size()) {
    m_callback->end(m_return_dest, m_returns);
    return;
  }
  ezInstruction *inst = m_carousel->instruction[m_pc++];
  inst->process(*this);
}

void ezStackFrame::on_mark(void) {
  for (vector<ezValue *>::iterator it = m_local->begin(); it != m_local->end();
       it++)
    (*it)->mark();
  for (vector<ezValue *>::iterator it = m_returns.begin();
       it != m_returns.end(); it++)
    (*it)->mark();
}
