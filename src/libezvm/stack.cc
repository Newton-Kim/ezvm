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
#include "ezvm/ezmemory.h"
#include "ezvm/ezstack.h"
#include <iostream>
#include <stdexcept>

ezStackFrame::ezStackFrame(ezFunction *crsl, vector<ezObject *> &args,
                           ezAddress &ret, ezStackFrameCallback *callback)
    : m_pc(0), m_local(crsl->local_memory()), m_scope(crsl->scope_memory()),
      m_return(NULL), m_carousel(crsl), m_callback(callback), m_alu(NULL) {
  if (!m_callback)
    throw runtime_error("callback is null.");
  size_t min_args = (crsl->nargs > args.size()) ? args.size() : crsl->nargs;
  for (size_t i = 0; i < min_args; i++)
    (*m_local)[i] = args[i];
  m_return_dest = ret;
  for (size_t i = 0; i < m_carousel->ntemps; i++)
    m_temporary.push_back(ezNull::instance());
  m_memory.push_back(&(ezMemory::instance().globals().to_vector()));
  m_memory.push_back(&ezMemory::instance().constants());
  m_memory.push_back(m_local);
  m_memory.push_back(&m_temporary);
  m_memory.push_back(m_scope);
  m_alu = ezALUImplementor::instance()->get_alu();
  if(!m_alu) throw runtime_error("ALU is missing");
}

ezStackFrame::ezStackFrame(ezFunction *crsl, vector<ezObject *> &args,
                           ezStackFrameCallback *callback)
    : m_pc(0), m_local(m_carousel->local_memory()),
      m_scope(m_carousel->scope_memory()), m_carousel(crsl),
      m_callback(callback) {
  if (!m_callback)
    throw runtime_error("callback is null.");
  size_t min_args = (crsl->nargs > args.size()) ? args.size() : crsl->nargs;
  for (size_t i = 0; i < min_args; i++)
    (*m_local)[i] = args[i];
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
  for (size_t i = 0; i < addr.size(); i++) {
    ezObject *v = addr2val(addr[i]);
    if(v->type == EZ_OBJECT_TYPE_ARRAY) {
      ezArray * arr = (ezArray*) v;
      for(size_t j = 0; j < arr->data.size(); j++)
        vals.push_back(arr->data[j]);
    } else {
      vals.push_back(v);
    }
  }
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
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->add(vl, vr);
  });
}

void ezStackFrame::add(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->add(vl, vr);
      });
}

void ezStackFrame::sub(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->subtract(vl, vr);
  });
}

void ezStackFrame::sub(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->subtract(vl, vr);
      });
}

void ezStackFrame::mul(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->multiply(vl, vr);
  });
}

void ezStackFrame::mul(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->multiply(vl, vr);
      });
}

void ezStackFrame::div(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->divide(vl, vr);
  });
}

void ezStackFrame::div(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->divide(vl, vr);
      });
}

void ezStackFrame::mod(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->modulate(vl, vr);
  });
}

void ezStackFrame::mod(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->modulate(vl, vr);
      });
}

void ezStackFrame::cmp(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  ezValue *vl = NULL, *vr = NULL;
  ezObject *rst = NULL;
  vl = (ezValue *)addr2val(src1);
  vr = (ezValue *)addr2val(src2);
  rst = m_alu->compare(vl, vr);
  val2addr(dest, rst);
}

void ezStackFrame::pwr(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->power(vl, vr);
  });
}

void ezStackFrame::pwr(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->power(vl, vr);
      });
}

void ezStackFrame::b_and(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->bitwise_and(vl, vr);
  });
}

void ezStackFrame::b_and(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                         ezAddress &src2) {
  calculate_binary(dest, cond, src1, src2,
                   [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
                     return alu->bitwise_and(vl, vr);
                   });
}

void ezStackFrame::b_or(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->bitwise_or(vl, vr);
  });
}

void ezStackFrame::b_or(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                        ezAddress &src2) {
  calculate_binary(dest, cond, src1, src2,
                   [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
                     return alu->bitwise_or(vl, vr);
                   });
}

void ezStackFrame::b_xor(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->bitwise_xor(vl, vr);
  });
}

void ezStackFrame::b_xor(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                         ezAddress &src2) {
  calculate_binary(dest, cond, src1, src2,
                   [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
                     return alu->bitwise_xor(vl, vr);
                   });
}

void ezStackFrame::lsl(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->lsl(vl, vr);
  });
}

void ezStackFrame::lsl(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->lsl(vl, vr);
      });
}

void ezStackFrame::lsr(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  calculate_binary(dest, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
    return alu->lsr(vl, vr);
  });
}

void ezStackFrame::lsr(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  calculate_binary(
      dest, cond, src1, src2, [](ezALU * alu, ezValue *vl, ezValue *vr) -> ezValue * {
        return alu->lsr(vl, vr);
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
  ezObject *cond = m_alu->compare(vl, vr);
  val2addr(rst, func((ezCondition *)cond));
  delete cond;
}

void ezStackFrame::test_equality(ezAddress &rst, ezAddress &cond,
                                 ezAddress &lsrc, ezAddress &rsrc,
                                 function<ezValue *(ezCondition *)> func) {
  ezValue *vr = NULL, *vl = NULL;
  vl = (ezValue *)addr2val(lsrc);
  vr = (ezValue *)addr2val(rsrc);
  ezObject *vcond = m_alu->compare(vl, vr);
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
    function<ezValue *(ezALU *, ezValue *, ezValue *)> func) {
  ezValue *vl = NULL, *vr = NULL, *rst = NULL;
  vl = (ezValue *)addr2val(src1);
  vr = (ezValue *)addr2val(src2);
  rst = func(m_alu, vl, vr);
  val2addr(dest, rst);
}

void ezStackFrame::calculate_binary(
    ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2,
    function<ezValue *(ezALU *, ezValue *, ezValue *)> func) {
  ezValue *vl = NULL, *vr = NULL, *rst = NULL;
  vl = (ezValue *)addr2val(src1);
  vr = (ezValue *)addr2val(src2);
  rst = func(m_alu, vl, vr);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::calculate_unary(ezAddress &dest, ezAddress &src,
                                   function<ezValue *(ezALU*, ezValue *)> func) {
  ezValue *v = NULL, *rst = NULL;
  v = (ezValue *)addr2val(src);
  rst = func(m_alu, v);
  val2addr(dest, rst);
}

void ezStackFrame::calculate_unary(ezAddress &dest, ezAddress &cond,
                                   ezAddress &src,
                                   function<ezValue *(ezALU*, ezValue *)> func) {
  ezValue *v = NULL, *rst = NULL;
  v = (ezValue *)addr2val(src);
  rst = func(m_alu, v);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::negate(ezAddress &dest, ezAddress &src) {
  calculate_unary(dest, src,
                  [](ezALU* alu, ezValue *v) -> ezValue * { return alu->negate(v); });
}

void ezStackFrame::negate(ezAddress &dest, ezAddress &cond, ezAddress &src) {
  calculate_unary(dest, cond, src,
                  [](ezALU* alu, ezValue *v) -> ezValue * { return alu->negate(v); });
}

void ezStackFrame::b_not(ezAddress &dest, ezAddress &src) {
  calculate_unary(dest, src,
                  [](ezALU* alu, ezValue *v) -> ezValue * { return alu->bitwise_not(v); });
}

void ezStackFrame::b_not(ezAddress &dest, ezAddress &cond, ezAddress &src) {
  calculate_unary(dest, cond, src,
                  [](ezALU* alu, ezValue *v) -> ezValue * { return alu->bitwise_not(v); });
}

void ezStackFrame::fgc(void) { ezGC::instance().force(); }

void ezStackFrame::ret(vector<ezAddress> &srcs) {
  ezAddress dest, addr, cond;
  if (srcs.size() == 0) {
    m_return = NULL;
  } else if (srcs.size() == 1) {
    m_return = addr2val(srcs[0]);
  } else {
    ezObject *v = NULL;
    ezArray *array = new ezArray;
    for (size_t i = 0; i < srcs.size(); i++) {
      EZ_INFO("0x%x<-0x%x[%d]", array, srcs, i);
      v = addr2val(srcs[i]);
      array->data.push_back(v);
    }
    m_return = array;
    ezGC::instance().add(m_return);
  }
  m_callback->end(m_return_dest, m_return);
}

void ezStackFrame::mv(vector<ezAddress> &dests, vector<ezAddress> &srcs) {
  size_t cnt = dests.size();
  ezObject *v = NULL;
  size_t i = 0;
  vector<ezObject *> q;
  addr2val(q, srcs);
  for (i = 0; i < cnt && i < q.size(); i++) {
    EZ_INFO("%d:%d <= 0x%x", dests[i].segment, dests[i].offset, q[i]);
    val2addr(dests[i], q[i]);
  }
  while (i < cnt) {
    EZ_INFO("%d:%d <== 0x%x", dests[i].segment, dests[i].offset, ezNull::instance());
    val2addr(dests[i++], ezNull::instance());
  }
}

void ezStackFrame::call(ezAddress &func, vector<ezAddress> &args,
                        ezAddress &ret) {
  ezObject *proc = addr2val(func);
  vector<ezObject *> vargs;
  addr2val(vargs, args);
  switch (proc->type) {
  case EZ_OBJECT_TYPE_USER_DEFINED_FUNCTION:
    call((ezUserDefinedFunction *)proc, vargs, ret);
    break;
  case EZ_OBJECT_TYPE_FUNCTION:
    call((ezFunction *)proc, vargs, ret);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
}

void ezStackFrame::call(ezAddress &func, vector<ezAddress> &args) {
  ezObject *proc = addr2val(func);
  vector<ezObject *> vargs;
  addr2val(vargs, args);
  switch (proc->type) {
  case EZ_OBJECT_TYPE_USER_DEFINED_FUNCTION:
    call((ezUserDefinedFunction *)proc, vargs);
    break;
  case EZ_OBJECT_TYPE_FUNCTION:
    call((ezFunction *)proc, vargs);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
}

void ezStackFrame::call(ezFunction *func, vector<ezObject *> &args,
                        ezAddress &ret) {
  ezGC::instance().pause();
  ezStackFrame *callee = new ezStackFrame(func, args, ret, m_callback);
  m_callback->call(callee);
  ezGC::instance().resume();
}

void ezStackFrame::call(ezFunction *func, vector<ezObject *> &args) {
  ezGC::instance().pause();
  ezStackFrame *callee = new ezStackFrame(func, args, m_callback);
  m_callback->call(callee);
  ezGC::instance().resume();
}

void ezStackFrame::call(ezUserDefinedFunction *func, vector<ezObject *> &args,
                        ezAddress &ret) {
  ezObject *vret = func->run(args);
  if (vret)
    val2addr(ret, vret);
}

void ezStackFrame::call(ezUserDefinedFunction *func, vector<ezObject *> &args) {
  func->run(args);
}

void ezStackFrame::thd(ezAddress &func, vector<ezAddress> &args, ezAddress &ret,
                       ezAddress &handle) {
  vector<ezObject *> vargs;
  addr2val(vargs, args);
  size_t hthd = m_callback->thd(func, vargs, ret, this);
  val2addr(handle, new ezHandle(hthd));
}

void ezStackFrame::thd(ezAddress &func, vector<ezAddress> &args,
                       ezAddress &handle) {
  vector<ezObject *> vargs;
  addr2val(vargs, args);
  size_t hthd = m_callback->thd(func, vargs, this);
  val2addr(handle, new ezHandle(hthd));
}

void ezStackFrame::wait(ezAddress &handle) {
  ezObject *v = addr2val(handle);
  if (v->type != EZ_OBJECT_TYPE_HANDLE)
    throw runtime_error("invalid handle");
  m_callback->wait(((ezHandle *)v)->id);
}

void ezStackFrame::update(ezAddress &dest, ezObject *val) {
  if (val)
    val2addr(dest, val);
}

void ezStackFrame::step(void) {
  if (m_pc >= m_carousel->instruction.size()) {
    m_callback->end(m_return_dest, m_return);
    return;
  }
  ezInstruction *inst = m_carousel->instruction[m_pc++];
  inst->process(*this);
}

void ezStackFrame::on_mark(void) {
  EZ_INFO("start");
  EZ_INFO("m_local size: %d", m_local->size());
  for (vector<ezObject *>::iterator it = m_local->begin(); it != m_local->end();
       it++) {
    ezObject *obj = *it;
    EZ_INFO("visiting 0x%x", obj);
    if (obj)
      obj->mark();
  }
  if (m_return) {
    EZ_INFO("marking return value of 0x%x", m_return);
    m_return->mark();
  }
  EZ_INFO("end");
}

void ezStackFrame::dump(ezFile &sink) {
  sink.print("    .stk_0x%x:\n", this);
  sink.print("      PC:%lu\n", m_pc);
  sink.print("      context:0x%x\n", m_carousel);
  sink.print("      .local memory:\n");
  size_t i = 0;
  for (vector<ezObject *>::iterator it = m_local->begin(); it != m_local->end();
       it++) {
    sink.print("        [%d]:", i++);
    (*it)->dump(sink);
  }
  sink.print("      .temporary memory:\n");
  i = 0;
  for (vector<ezObject *>::iterator it = m_temporary.begin();
       it != m_temporary.end(); it++) {
    sink.print("        [%d]:", i++);
    (*it)->dump(sink);
  }
}
