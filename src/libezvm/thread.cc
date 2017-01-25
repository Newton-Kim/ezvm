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
#include "ezvm/ezthread.h"
#include <stdexcept>

ezThread::ezThread(ezAddress entry, ezTable<string, ezValue *> &globals,
                   vector<ezValue *> &constants, ezALU &alu, ezGC &gc)
    : m_entry(entry), m_constants(constants), m_globals(globals), m_alu(alu),
      m_gc(gc) {
  ezValue *v = addr2val(entry);
  switch (v->type) {
  case EZ_VALUE_TYPE_CAROUSEL: {
    ezCarousel *crsl = (ezCarousel *)v;
    ezStackFrame *sf = new ezStackFrame(crsl, m_globals, m_constants, m_alu, m_gc);
    for (size_t i = 0; i < crsl->nmems; i++)
      sf->local.push_back(ezNull::instance());
    m_stack.push_back(sf);
  } break;
  case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
    ((ezNativeCarousel *)v)->run(m_args, m_rets);
    break;
  default:
    throw("invalid value type");
    break;
  }
}

ezThread::~ezThread() {
  while (!m_stack.empty()) {
    ezStackFrame *sf = m_stack.back();
    if (sf)
      delete sf;
    m_stack.pop_back();
  }
}

ezStepState ezThread::step(void) {
  // It needs 3 steps to finalize a thread
  if (m_stack.empty())
    return EZ_STEP_DONE;
  ezStackFrame *sf = m_stack.back();
  ezLog &log = ezLog::instance();
  ezOpCode op;
  uint8_t arg1, arg2, arg3;
  if (EZ_STEP_DONE == sf->step(op, arg1, arg2, arg3)) {
    log.verbose("stack %p has poped out", sf);
    m_stack.pop_back();
    size_t rets = sf->returns.size();
    size_t dests = sf->return_dest.size();
    size_t cnt = (rets > dests) ? dests : rets;
    for (size_t i = 0; i < cnt; i++)
      val2addr(sf->return_dest[i], sf->returns[i]);
    if (dests > rets) {
      for (size_t i = cnt; i < dests; i++)
        val2addr(sf->return_dest[i], ezNull::instance());
    }
    delete sf;
    return EZ_STEP_CONTINUE;
  }
  log.verbose("stack %p has turn", sf);
  switch (op) {
  case EZ_OP_CALL:
    call(arg1, arg2);
    break;
  }
  return EZ_STEP_CONTINUE;
}

void ezThread::call(uint8_t nargs, uint8_t nrets) {
  // TODO it can be done via a macro
  if (m_stack.empty())
    throw runtime_error("stack underrun");
  ezStackFrame *sf = m_stack.back();
  ezInstDecoder decoder;
  ezAddress addr;
  decoder.argument(sf->carousel->instruction[sf->pc++], addr);
  ezValue *func = addr2val(addr);
  switch (func->type) {
  case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
    call((ezNativeCarousel *)func, nargs, nrets);
    break;
  case EZ_VALUE_TYPE_CAROUSEL:
    call((ezCarousel *)func, nargs, nrets);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
}

void ezThread::call(ezNativeCarousel *func, uint8_t nargs, uint8_t nrets) {
  ezStackFrame *sf = m_stack.back();
  ezInstDecoder decoder;
  vector<ezValue *> args;
  ezAddress addr;
  for (size_t i = 0; i < nargs; i++, sf->pc++) {
    decoder.argument(sf->carousel->instruction[sf->pc], addr);
    ezValue *v = addr2val(addr);
    args.push_back(v);
  }
  vector<ezAddress> ret_dest;
  for (size_t i = 0; i < nrets; i++, sf->pc++) {
    decoder.argument(sf->carousel->instruction[sf->pc], addr);
    ret_dest.push_back(addr);
  }
  vector<ezValue *> rets;
  func->run(args, rets);
  if (rets.size())
    val2addr(ret_dest, rets);
}

void ezThread::call(ezCarousel *func, uint8_t nargs, uint8_t nrets) {
  ezStackFrame *nsf = new ezStackFrame(func, m_globals, m_constants, m_alu, m_gc);
  ezStackFrame *sf = m_stack.back();
  ezInstDecoder decoder;
  vector<ezValue *> args;
  ezAddress addr;
  size_t min_args = (func->nargs > nargs) ? nargs : func->nargs;
  for (size_t i = 0; i < min_args; i++, sf->pc++) {
    decoder.argument(sf->carousel->instruction[sf->pc], addr);
    ezValue *v = addr2val(addr);
    nsf->local.push_back(v);
  }
  if (func->nargs > nargs) {
    for (size_t i = min_args; i < func->nargs; i++, sf->pc++) {
      nsf->local.push_back(ezNull::instance());
    }
  }
  for (size_t i = 0; i < func->nmems; i++)
    nsf->local.push_back(ezNull::instance());
  vector<ezAddress> ret_dest;
  for (size_t i = 0; i < nrets; i++, sf->pc++) {
    decoder.argument(sf->carousel->instruction[sf->pc], addr);
    nsf->return_dest.push_back(addr);
  }
  m_stack.push_back(nsf);
}

ezValue *ezThread::addr2val(ezAddress addr) {
  ezValue *v = NULL;
  switch (addr.segment) {
  case EZ_ASM_SEGMENT_CONSTANT:
    if (addr.offset >= m_constants.size())
      throw runtime_error("constant memory access violation");
    v = m_constants[addr.offset];
    break;
  case EZ_ASM_SEGMENT_LOCAL: {
    ezStackFrame *sf = m_stack.back();
    if (addr.offset >= sf->local.size())
      throw runtime_error("local memory access violation");
    v = sf->local[addr.offset];
  } break;
  case EZ_ASM_SEGMENT_PARENT:
    throw runtime_error("parent segment has not been implemented");
    break;
  case EZ_ASM_SEGMENT_GLOBAL:
    if (addr.offset >= m_globals.m_memory.size())
      throw runtime_error("global memory access violation");
    v = m_globals.m_memory[addr.offset];
    break;
  default:
    throw runtime_error("out of segment boundary");
  }
  return v;
}

void ezThread::val2addr(vector<ezAddress> &addr, vector<ezValue *> &vals) {
  size_t addr_sz = addr.size(), vals_sz = vals.size();
  size_t gcv = (addr_sz > vals_sz) ? vals_sz : addr_sz;
  for (size_t i = 0; i < gcv; i++) {
    ezValue *v = vals[i];
    val2addr(addr[i], v);
  }
  if (addr_sz > vals_sz) {
    for (size_t i = gcv; i < addr_sz; i++)
      val2addr(addr[i], ezNull::instance());
  }
}

void ezThread::val2addr(ezAddress addr, ezValue *v) {
  switch (addr.segment) {
  case EZ_ASM_SEGMENT_CONSTANT:
    throw runtime_error("cannot write to constant");
    break;
  case EZ_ASM_SEGMENT_LOCAL: {
    ezStackFrame *sf = m_stack.back();
    if (addr.offset >= sf->local.size())
      throw runtime_error("local memory access violation");
    sf->local[addr.offset] = v;
  } break;
  case EZ_ASM_SEGMENT_PARENT:
    throw runtime_error("parent segment has not been implemented");
    break;
  case EZ_ASM_SEGMENT_GLOBAL:
    if (addr.offset >= m_globals.m_memory.size())
      throw runtime_error("global memory access violation");
    m_globals.m_memory[addr.offset] = v;
    break;
    break;
    throw runtime_error("out of segment boundary");
    break;
  }
}

void ezThread::on_mark(void) {
  for (vector<ezStackFrame *>::iterator it = m_stack.begin();
       it != m_stack.end(); it++) {
    ezStackFrame *stk = *it;
    for (vector<ezValue *>::iterator lit = stk->local.begin();
         lit != stk->local.end(); lit++)
      (*lit)->mark();
    for (vector<ezValue *>::iterator lit = stk->returns.begin();
         lit != stk->returns.end(); lit++)
      (*lit)->mark();
  }
}
