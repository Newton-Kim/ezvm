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
#include "ezvm/ezlog.h"
#include "ezvm/ezstack.h"
#include <iostream>
#include <stdexcept>

ezStackFrame::ezStackFrame(ezCarousel *crsl, ezStackFrameCallback *callback,
                           ezTable<string, ezValue *> &globals,
                           vector<ezValue *> &constants)
    : m_pc(0), m_local(m_carousel->local_memory()),
      m_scope(m_carousel->scope_memory()), m_carousel(crsl),
      m_constants(constants), m_globals(globals), m_alu(ezALU::instance()),
      m_callback(callback) {
  if (!m_callback)
    throw runtime_error("callback is null.");
  ezLog::instance().verbose("%s", __PRETTY_FUNCTION__);
  m_memory.push_back(&(m_globals.m_memory));
  m_memory.push_back(&m_constants);
  m_memory.push_back(m_local);
  m_memory.push_back(m_scope);
}

ezStackFrame::~ezStackFrame() {
  if (!m_carousel->is_local_scoped())
    delete m_local;
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
  if (v != ezNull::instance())
    ezGC::instance().add((ezGCObject *)v);
}

void ezStackFrame::shift_operation(
    ezAddress &dest, ezAddress &src, ezAddress &offset,
    function<ezValue *(ezValue *, ezValue *)> func) {
  ezValue *rst = NULL, *obj = NULL, *shift = NULL;
  obj = addr2val(src);
  shift = addr2val(offset);
  rst = func(obj, shift);
  val2addr(dest, rst);
}

void ezStackFrame::shift_operation(
    ezAddress &dest, ezAddress &cond, ezAddress &src, ezAddress &offset,
    function<ezValue *(ezValue *, ezValue *)> func) {
  ezValue *rst = NULL, *obj = NULL, *shift = NULL;
  obj = addr2val(src);
  shift = addr2val(offset);
  rst = func(obj, shift);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::lsl(ezAddress &dest, ezAddress &src, ezAddress &offset) {
  shift_operation(dest, src, offset, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() << offset->to_integer());
  });
}

void ezStackFrame::lsl(ezAddress &dest, ezAddress &cond, ezAddress &src,
                       ezAddress &offset) {
  shift_operation(dest, cond, src, offset, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() << offset->to_integer());
  });
}

void ezStackFrame::lsr(ezAddress &dest, ezAddress &src, ezAddress &offset) {
  shift_operation(dest, src, offset, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() >> offset->to_integer());
  });
}

void ezStackFrame::lsr(ezAddress &dest, ezAddress &cond, ezAddress &src,
                       ezAddress &offset) {
  shift_operation(dest, cond, src, offset, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() >> offset->to_integer());
  });
}

void ezStackFrame::binary_operation(
    ezAddress &dest, ezAddress &src1, ezAddress &src2,
    function<ezValue *(ezValue *, ezValue *)> binary_func) {
  ezValue *rst = NULL;
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(src1);
  vr = addr2val(src2);
  rst = binary_func(vl, vr);
  val2addr(dest, rst);
}

void ezStackFrame::binary_operation(
    ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2,
    function<ezValue *(ezValue *, ezValue *)> binary_func) {
  ezValue *rst = NULL;
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(src1);
  vr = addr2val(src2);
  rst = binary_func(vl, vr);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::add(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.add(vl, vr); });
}

void ezStackFrame::add(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.add(vl, vr); });
}

void ezStackFrame::div(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.div(vl, vr); });
}

void ezStackFrame::div(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.div(vl, vr); });
}

void ezStackFrame::mod(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mod(vl, vr); });
}

void ezStackFrame::mod(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mod(vl, vr); });
}

void ezStackFrame::mul(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mul(vl, vr); });
}

void ezStackFrame::mul(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mul(vl, vr); });
}

void ezStackFrame::sub(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.sub(vl, vr); });
}

void ezStackFrame::sub(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.sub(vl, vr); });
}

void ezStackFrame::bitwise_and(ezAddress &dest, ezAddress &src1,
                               ezAddress &src2) {
  binary_operation(dest, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_and(vl, vr);
  });
}

void ezStackFrame::bitwise_and(ezAddress &dest, ezAddress &cond,
                               ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, cond, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_and(vl, vr);
  });
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

void ezStackFrame::bitwise_or(ezAddress &dest, ezAddress &src1,
                              ezAddress &src2) {
  binary_operation(dest, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_or(vl, vr);
  });
}

void ezStackFrame::bitwise_or(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                              ezAddress &src2) {
  binary_operation(dest, cond, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_or(vl, vr);
  });
}

ezValue *ezStackFrame::test_equality(ezAddress &rst, ezAddress &lsrc,
                                     ezAddress &rsrc,
                                     function<ezValue *(ezCondition *)> func) {
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(lsrc);
  vr = addr2val(rsrc);
  ezValue *cond = m_alu.cmp(vl, vr);
  val2addr(rst, func((ezCondition *)cond));
  return cond;
}

void ezStackFrame::teq(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  ezValue *cond = test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->zero) ? new ezBool(true) : new ezBool(false);
  });
  delete cond;
}

void ezStackFrame::teq(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  ezValue *vcond = test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->zero) ? new ezBool(true) : new ezBool(false);
  });
  val2addr(cond, vcond);
}

void ezStackFrame::tge(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  ezValue *cond = test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->zero || !cond->negative) ? new ezBool(true)
                                           : new ezBool(false);
  });
  delete cond;
}

void ezStackFrame::tge(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  ezValue *vcond = test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->zero || !cond->negative) ? new ezBool(true)
                                           : new ezBool(false);
  });
  val2addr(cond, vcond);
}

void ezStackFrame::tlt(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  ezValue *cond = test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->negative) ? new ezBool(true) : new ezBool(false);
  });
  delete cond;
}

void ezStackFrame::tlt(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  ezValue *vcond = test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (cond->negative) ? new ezBool(true) : new ezBool(false);
  });
  val2addr(cond, vcond);
}

void ezStackFrame::tne(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  ezValue *cond = test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (!cond->zero) ? new ezBool(true) : new ezBool(false);
  });
  delete cond;
}

void ezStackFrame::tne(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  ezValue *vcond = test_equality(dest, src1, src2, [](ezCondition *cond) {
    return (!cond->zero) ? new ezBool(true) : new ezBool(false);
  });
  val2addr(cond, vcond);
}

void ezStackFrame::bitwise_xor(ezAddress &dest, ezAddress &src1,
                               ezAddress &src2) {
  binary_operation(dest, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_xor(vl, vr);
  });
}

void ezStackFrame::bitwise_xor(ezAddress &dest, ezAddress &cond,
                               ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, cond, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_xor(vl, vr);
  });
}

void ezStackFrame::unary_operation(ezAddress &dest, ezAddress &src,
                                   function<ezValue *(ezValue *)> unary_func) {
  ezValue *v = NULL, *rst = NULL;
  v = addr2val(src);
  rst = unary_func(v);
  val2addr(dest, rst);
}

void ezStackFrame::unary_operation(ezAddress &dest, ezAddress &cond,
                                   ezAddress &src,
                                   function<ezValue *(ezValue *)> unary_func) {
  ezValue *v = NULL, *rst = NULL;
  v = addr2val(src);
  rst = unary_func(v);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::neg(ezAddress &dest, ezAddress &src) {
  unary_operation(dest, src, [&](ezValue *v) { return m_alu.neg(v); });
}

void ezStackFrame::neg(ezAddress &dest, ezAddress &cond, ezAddress &src) {
  unary_operation(dest, cond, src, [&](ezValue *v) { return m_alu.neg(v); });
}

void ezStackFrame::bitwise_not(ezAddress &dest, ezAddress &src) {
  unary_operation(dest, src, [&](ezValue *v) { return m_alu.bitwise_not(v); });
}

void ezStackFrame::bitwise_not(ezAddress &dest, ezAddress &cond,
                               ezAddress &src) {
  unary_operation(dest, cond, src,
                  [&](ezValue *v) { return m_alu.bitwise_not(v); });
}

void ezStackFrame::fgc(void) { ezGC::instance().force(); }

void ezStackFrame::ret(vector<ezAddress> &srcs) {
  ezAddress dest, addr, cond;
  ezValue *v = NULL;
  for (size_t i = 0; i < srcs.size(); i++) {
    v = addr2val(srcs[i]);
    m_returns.push_back(v);
  }
}

void ezStackFrame::mv(vector<ezAddress> &dests, vector<ezAddress> &srcs) {
  size_t cnt = (dests.size() > srcs.size()) ? srcs.size() : dests.size();
  ezValue *v = NULL;
  size_t i = 0;
  vector<ezValue *> q;
  for (i = 0; i < cnt; i++) {
    v = addr2val(srcs[i]);
    q.push_back(v);
  }
  for (i = 0; i < cnt; i++) {
    val2addr(dests[i], q[i]);
  }
  if (dests.size() > srcs.size()) {
    for (i = cnt; i < dests.size(); i++) {
      val2addr(dests[i], ezNull::instance());
    }
  }
}

void ezStackFrame::cmp(ezAddress &cond, ezAddress &src1, ezAddress &src2) {
  ezValue *rst = NULL, *vr = NULL, *vl = NULL;
  vl = addr2val(src1);
  vr = addr2val(src2);
  rst = m_alu.cmp(vl, vr);
  val2addr(cond, rst);
}

void ezStackFrame::call(ezAddress &func, vector<ezAddress> &args,
                        vector<ezAddress> &rets) {
  ezValue *proc = addr2val(func);
  switch (proc->type) {
  case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
    call((ezNativeCarousel *)proc, args, rets);
    break;
  case EZ_VALUE_TYPE_CAROUSEL:
    call((ezCarousel *)proc, args, rets);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
}

void ezStackFrame::call(ezCarousel *func, vector<ezAddress> &args,
                        vector<ezAddress> &rets) {
  ezStackFrame *callee =
      new ezStackFrame(func, m_callback, m_globals, m_constants);
  ezAddress addr;
  size_t min_args = (func->nargs > args.size()) ? args.size() : func->nargs;
  for (size_t i = 0; i < min_args; i++) {
    ezValue *v = addr2val(args[i]);
    (*(callee->m_local))[i] = v;
  }
  for (size_t i = 0; i < rets.size(); i++) {
    callee->m_return_dest.push_back(rets[i]);
  }
  m_callback->call(callee);
}

void ezStackFrame::call(ezNativeCarousel *func, vector<ezAddress> &args,
                        vector<ezAddress> &rets) {
  vector<ezValue *> vargs;
  for (size_t i = 0; i < args.size(); i++) {
    ezValue *v = addr2val(args[i]);
    vargs.push_back(v);
  }
  vector<ezValue *> vrets;
  func->run(vargs, vrets);
  if (rets.size())
    val2addr(rets, vrets);
}

void ezStackFrame::thd(ezAddress &func, vector<ezAddress> &args,
                       vector<ezAddress> &rets, ezAddress &handle) {
  size_t hthd = m_callback->thd(func, args, rets);
  val2addr(handle, new ezInteger(hthd));
}

void ezStackFrame::wait(ezAddress &handle) {
  ezValue *v = addr2val(handle);
  if (v->type != EZ_VALUE_TYPE_INTEGER)
    throw runtime_error("invalid handle");
  m_callback->wait(((ezInteger *)v)->to_integer());
}

void ezStackFrame::update(ezStackFrame *sf) {
  size_t rets = sf->m_returns.size();
  size_t dests = sf->m_return_dest.size();
  size_t cnt = (rets > dests) ? dests : rets;
  for (size_t i = 0; i < cnt; i++)
    val2addr(sf->m_return_dest[i], sf->m_returns[i]);
  if (dests > rets) {
    for (size_t i = cnt; i < dests; i++)
      val2addr(sf->m_return_dest[i], ezNull::instance());
  }
}

void ezStackFrame::step(void) {
  if (m_pc >= m_carousel->instruction.size()) {
    m_callback->end();
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
