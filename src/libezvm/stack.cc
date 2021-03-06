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
                           ezAddress &ret, ezStackFrame *caller,
                           ezStackFrameCallback *callback)
    : m_pc(0), m_local(NULL), m_scope(NULL), m_return(NULL), m_carousel(crsl),
      m_callback(callback) {
  initialise(caller, args);
  m_return_dest = ret;
}

ezStackFrame::ezStackFrame(ezFunction *crsl, vector<ezObject *> &args,
                           ezStackFrame *caller, ezStackFrameCallback *callback)
    : m_pc(0), m_local(NULL), m_scope(NULL), m_carousel(crsl),
      m_callback(callback) {
  initialise(caller, args);
}

ezStackFrame::~ezStackFrame() {}

void ezStackFrame::initialise(ezStackFrame *caller, vector<ezObject *> &args) {
  {
    size_t memories = m_carousel->nmems + m_carousel->nargs;
    m_local = new ezMemSegment(memories);
    ezGC::instance().add((ezGCObject *)m_local);
  }
  if (m_carousel->is_scoped()) {
    if (!caller)
      throw runtime_error("stack is not scoped.");
    m_scope = caller->m_local;
  }
  if (!m_callback)
    throw runtime_error("callback is null.");
  size_t min_args =
      (m_carousel->nargs > args.size()) ? args.size() : m_carousel->nargs;
  for (size_t i = 0; i < min_args; i++)
    (*m_local->to_vector())[i] = args[i];
  for (size_t i = 0; i < m_carousel->ntemps; i++)
    m_temporary.push_back(ezNull::instance());
  m_memory.push_back(&(ezMemory::instance()->globals().to_vector()));
  m_memory.push_back(&ezMemory::instance()->constants());
  m_memory.push_back(m_local->to_vector());
  m_memory.push_back(&m_temporary);
  if (m_scope)
    m_memory.push_back(m_scope->to_vector());
}

void ezStackFrame::addr2val(vector<ezObject *> &vals, vector<ezAddress> &addr) {
  for (size_t i = 0; i < addr.size(); i++) {
    ezObject *v = addr2val(addr[i]);
    if (v->type == EZ_OBJECT_TYPE_ARRAY) {
      ezArray *arr = (ezArray *)v;
      for (size_t j = 0; j < arr->data.size(); j++)
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

void ezStackFrame::bra(size_t index) {
  if (m_carousel->jmptbl.size() <= index)
    throw runtime_error("tried to jump to an invalid label");
  m_pc = m_carousel->jmptbl[index];
}

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

void ezStackFrame::call(ezFunction *func, vector<ezObject *> &args,
                        ezAddress &ret) {
  ezGC::instance().pause();
  ezStackFrame *callee = new ezStackFrame(func, args, ret, this, m_callback);
  m_callback->call(callee);
  ezGC::instance().add((ezGCObject *)callee);
  ezGC::instance().resume();
}

void ezStackFrame::call(ezFunction *func, vector<ezObject *> &args) {
  ezGC::instance().pause();
  ezStackFrame *callee = new ezStackFrame(func, args, this, m_callback);
  m_callback->call(callee);
  ezGC::instance().add((ezGCObject *)callee);
  ezGC::instance().resume();
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

void ezStackFrame::step(void) {
  if (m_pc >= m_carousel->instruction.size()) {
    m_callback->end(m_return_dest, m_return);
    return;
  }
  EZ_INFO("PC:%d\n", m_pc);
  ezInstruction *inst = m_carousel->instruction[m_pc++];
  inst->process(*this);
}

void ezStackFrame::on_mark(void) {
  EZ_INFO("start");
  m_local->mark();
  if (m_scope) {
    m_scope->mark();
  }
  for (vector<ezObject *>::iterator it = m_temporary.begin();
       it != m_temporary.end(); it++) {
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
  sink.print("      PC:%lu\n", m_pc - 1);
  sink.print("      context:0x%x\n", m_carousel);
  sink.print("      .local memory:\n");
  size_t i = 0;
  for (vector<ezObject *>::iterator it = m_local->to_vector()->begin();
       it != m_local->to_vector()->end(); it++) {
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
