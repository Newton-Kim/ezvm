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

ezStackFrame::ezStackFrame(ezFunction *crsl, vector<ezObject *> &args,
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
  for (size_t i = 0; i < m_carousel->ntemps; i++)
    m_temporary.push_back(ezNull::instance());
  m_memory.push_back(&(ezMemory::instance().globals().to_vector()));
  m_memory.push_back(&ezMemory::instance().constants());
  m_memory.push_back(m_local);
  m_memory.push_back(&m_temporary);
  m_memory.push_back(m_scope);
}

ezStackFrame::~ezStackFrame() {
  if (!m_carousel->is_local_scoped())
    delete m_local;
}

void ezStackFrame::addr2val(vector<ezObject *> &vals, vector<ezAddress> &addr) {
  for (size_t i = 0; i < addr.size(); i++)
    vals.push_back(addr2val(addr[i]));
}

ezObject *ezStackFrame::addr2val(ezAddress addr) {
  return m_memory[addr.segment]->operator[](addr.offset);
}

void ezStackFrame::val2addr(vector<ezAddress> &addr, vector<ezObject *> &vals) {
  size_t addr_sz = addr.size(), vals_sz = vals.size();
  size_t gcv = (addr_sz > vals_sz) ? vals_sz : addr_sz;
  for (size_t i = 0; i < gcv; i++) {
    ezObject *v = vals[i];
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

void ezStackFrame::val2addr(ezAddress addr, ezObject *v) {
  m_memory[addr.segment]->operator[](addr.offset) = v;
  ezGC::instance().add((ezGCObject *)v);
}

void ezStackFrame::add(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->add(vr) : vr->add(vl, true);
  });
}

void ezStackFrame::add(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
        return (vl->id >= vr->id) ? vl->add(vr) : vr->add(vl, true);
      });
}

void ezStackFrame::sub(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->subtract(vr) : vr->subtract(vl, true);
  });
}

void ezStackFrame::sub(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
        return (vl->id >= vr->id) ? vl->subtract(vr) : vr->subtract(vl, true);
      });
}

void ezStackFrame::mul(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->multiply(vr) : vr->multiply(vl, true);
  });
}

void ezStackFrame::mul(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
        return (vl->id >= vr->id) ? vl->multiply(vr) : vr->multiply(vl, true);
      });
}

void ezStackFrame::div(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->divide(vr) : vr->divide(vl, true);
  });
}

void ezStackFrame::div(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
        return (vl->id >= vr->id) ? vl->divide(vr) : vr->divide(vl, true);
      });
}

void ezStackFrame::mod(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->modulate(vr) : vr->modulate(vl, true);
  });
}

void ezStackFrame::mod(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
        return (vl->id >= vr->id) ? vl->modulate(vr) : vr->modulate(vl, true);
      });
}

void ezStackFrame::cmp(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  ezValue *vl = NULL, *vr = NULL;
  ezObject *rst = NULL;
  vl = (ezValue *)addr2val(src1);
  vr = (ezValue *)addr2val(src2);
  rst = vl->compare(vr);
  val2addr(dest, rst);
}

void ezStackFrame::pwr(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->power(vr) : vr->power(vl, true);
  });
}

void ezStackFrame::pwr(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
        return (vl->id >= vr->id) ? vl->power(vr) : vr->power(vl, true);
      });
}

void ezStackFrame::b_and(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->bitwise_and(vr) : vr->bitwise_and(vl, true);
  });
}

void ezStackFrame::b_and(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                         ezAddress &src2) {
  calculate_binary(dest, cond, src1, src2,
                   [](ezValue *vl, ezValue *vr) -> ezValue * {
                     return (vl->id >= vr->id) ? vl->bitwise_and(vr)
                                               : vr->bitwise_and(vl, true);
                   });
}

void ezStackFrame::b_or(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->bitwise_or(vr) : vr->bitwise_or(vl, true);
  });
}

void ezStackFrame::b_or(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                        ezAddress &src2) {
  calculate_binary(dest, cond, src1, src2,
                   [](ezValue *vl, ezValue *vr) -> ezValue * {
                     return (vl->id >= vr->id) ? vl->bitwise_or(vr)
                                               : vr->bitwise_or(vl, true);
                   });
}

void ezStackFrame::b_xor(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->bitwise_xor(vr) : vr->bitwise_xor(vl, true);
  });
}

void ezStackFrame::b_xor(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                         ezAddress &src2) {
  calculate_binary(dest, cond, src1, src2,
                   [](ezValue *vl, ezValue *vr) -> ezValue * {
                     return (vl->id >= vr->id) ? vl->bitwise_xor(vr)
                                               : vr->bitwise_xor(vl, true);
                   });
}

void ezStackFrame::lsl(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->lsl(vr) : vr->lsl(vl, true);
  });
}

void ezStackFrame::lsl(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
        return (vl->id >= vr->id) ? vl->lsl(vr) : vr->lsl(vl, true);
      });
}

void ezStackFrame::lsr(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
    return (vl->id >= vr->id) ? vl->lsr(vr) : vr->lsr(vl, true);
  });
}

void ezStackFrame::lsr(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezValue *vl, ezValue *vr) -> ezValue * {
        return (vl->id >= vr->id) ? vl->lsr(vr) : vr->lsr(vl, true);
      });
}

void ezStackFrame::conditional_bra(ezAddress &cond, size_t index,
                                   function<bool(ezCondition *)> func) {
  ezObject *vcond = addr2val(cond);
  if (vcond->type != EZ_OBJECT_TYPE_CONDITION)
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
  vl = (ezValue *)addr2val(lsrc);
  vr = (ezValue *)addr2val(rsrc);
  ezObject *cond = vl->compare(vr);
  val2addr(rst, func((ezCondition *)cond));
  delete cond;
}

void ezStackFrame::test_equality(ezAddress &rst, ezAddress &cond,
                                 ezAddress &lsrc, ezAddress &rsrc,
                                 function<ezValue *(ezCondition *)> func) {
  ezValue *vr = NULL, *vl = NULL;
  vl = (ezValue *)addr2val(lsrc);
  vr = (ezValue *)addr2val(rsrc);
  ezObject *vcond = vl->compare(vr);
  val2addr(rst, func((ezCondition *)vcond));
  val2addr(cond, vcond);
}

/*
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
*/

void ezStackFrame::calculate_binary(
    ezAddress &dest, ezAddress &src1, ezAddress &src2,
    function<ezValue *(ezValue *, ezValue *)> func) {
  ezValue *vl = NULL, *vr = NULL, *rst = NULL;
  vl = (ezValue *)addr2val(src1);
  vr = (ezValue *)addr2val(src2);
  rst = func(vl, vr);
  val2addr(dest, rst);
}

void ezStackFrame::calculate_binary(
    ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2,
    function<ezValue *(ezValue *, ezValue *)> func) {
  ezValue *vl = NULL, *vr = NULL, *rst = NULL;
  vl = (ezValue *)addr2val(src1);
  vr = (ezValue *)addr2val(src2);
  rst = func(vl, vr);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::calculate_unary(ezAddress &dest, ezAddress &src,
                                   function<ezValue *(ezValue *)> func) {
  ezValue *v = NULL, *rst = NULL;
  v = (ezValue *)addr2val(src);
  rst = func(v);
  val2addr(dest, rst);
}

void ezStackFrame::calculate_unary(ezAddress &dest, ezAddress &cond,
                                   ezAddress &src,
                                   function<ezValue *(ezValue *)> func) {
  ezValue *v = NULL, *rst = NULL;
  v = (ezValue *)addr2val(src);
  rst = func(v);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::negate(ezAddress &dest, ezAddress &src) {
  calculate_unary(dest, src,
                  [](ezValue *v) -> ezValue * { return v->negate(); });
}

void ezStackFrame::negate(ezAddress &dest, ezAddress &cond, ezAddress &src) {
  calculate_unary(dest, cond, src,
                  [](ezValue *v) -> ezValue * { return v->negate(); });
}

void ezStackFrame::b_not(ezAddress &dest, ezAddress &src) {
  calculate_unary(dest, src,
                  [](ezValue *v) -> ezValue * { return v->bitwise_not(); });
}

void ezStackFrame::b_not(ezAddress &dest, ezAddress &cond, ezAddress &src) {
  calculate_unary(dest, cond, src,
                  [](ezValue *v) -> ezValue * { return v->bitwise_not(); });
}

void ezStackFrame::fgc(void) { ezGC::instance().force(); }

void ezStackFrame::ret(vector<ezAddress> &srcs) {
  ezAddress dest, addr, cond;
  ezObject *v = NULL;
  for (size_t i = 0; i < srcs.size(); i++) {
    v = addr2val(srcs[i]);
    m_returns.push_back(v);
  }
  m_callback->end(m_return_dest, m_returns);
}

void ezStackFrame::mv(vector<ezAddress> &dests, vector<ezAddress> &srcs) {
  size_t cnt = (dests.size() > srcs.size()) ? srcs.size() : dests.size();
  ezObject *v = NULL;
  size_t i = 0;
  vector<ezObject *> q;
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
  ezObject *proc = addr2val(func);
  vector<ezObject *> vargs;
  addr2val(vargs, args);
  switch (proc->type) {
  case EZ_OBJECT_TYPE_USER_DEFINED_FUNCTION:
    call((ezUserDefinedFunction *)proc, vargs, rets);
    break;
  case EZ_OBJECT_TYPE_FUNCTION:
    call((ezFunction *)proc, vargs, rets);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
}

void ezStackFrame::call(ezFunction *func, vector<ezObject *> &args,
                        vector<ezAddress> &rets) {
  ezGC::instance().pause();
  ezStackFrame *callee = new ezStackFrame(func, args, rets, m_callback);
  m_callback->call(callee);
  ezGC::instance().resume();
}

void ezStackFrame::call(ezUserDefinedFunction *func, vector<ezObject *> &args,
                        vector<ezAddress> &rets) {
  vector<ezObject *> vrets;
  func->run(args, vrets);
  if (rets.size())
    val2addr(rets, vrets);
}

void ezStackFrame::thd(ezAddress &func, vector<ezAddress> &args,
                       vector<ezAddress> &rets, ezAddress &handle) {
  vector<ezObject *> vargs;
  addr2val(vargs, args);
  size_t hthd = m_callback->thd(func, vargs, rets, this);
  val2addr(handle, new ezHandle(hthd));
}

void ezStackFrame::wait(ezAddress &handle) {
  ezObject *v = addr2val(handle);
  if (v->type != EZ_OBJECT_TYPE_HANDLE)
    throw runtime_error("invalid handle");
  m_callback->wait(((ezHandle *)v)->id);
}

void ezStackFrame::update(vector<ezAddress> &dests, vector<ezObject *> &vals) {
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
  for (vector<ezObject *>::iterator it = m_local->begin(); it != m_local->end();
       it++)
    (*it)->mark();
  for (vector<ezObject *>::iterator it = m_returns.begin();
       it != m_returns.end(); it++)
    (*it)->mark();
}
