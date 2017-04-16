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
#include "ezvm/ezgc.h"
#include "ezvm/ezinstruction.h"
#include "ezvm/ezlog.h"
#include "ezvm/ezmemory.h"
#include "ezvm/ezthread.h"
#include <stdexcept>

ezThread::ezThread(ezAddress entry, vector<ezValue*> &args,
                   vector<ezAddress> &rets, ezThreadCallback *callback,
                   ezThreadScheduler sched, ezStackFrame* caller)
    : m_entry(entry), m_scheduler(sched), m_wait(0),
      m_callback(callback), m_caller(caller) {
  if (!callback)
    throw runtime_error("callback is missing.");
  ezValue *v = addr2val(entry);
  // TODO:arguments and returns should be updated
  switch (v->type) {
  case EZ_VALUE_TYPE_CAROUSEL: {
    ezStackFrame *sf = new ezStackFrame((ezCarousel *)v, args, rets, this);
    m_stack.push_back(sf);
    ezGC::instance().add(sf);
  } break;
  case EZ_VALUE_TYPE_NATIVE_CAROUSEL: {
    vector<ezValue*> vrets;
    ((ezNativeCarousel *)v)->run(args, vrets);
    if(caller) caller->update(rets, vrets);
  } break;
  default:
    throw("invalid value type");
    break;
  }
}

ezThread::~ezThread() { }

ezValue *ezThread::addr2val(ezAddress addr) {
  // TODO:refactoring is required.
  ezValue *v = NULL;
  switch (addr.segment) {
  case EZ_ASM_SEGMENT_CONSTANT:
    if (addr.offset >= ezMemory::instance().constants().size())
      throw runtime_error("constant memory access violation");
    v = ezMemory::instance().constants()[addr.offset];
    break;
  case EZ_ASM_SEGMENT_LOCAL:
  case EZ_ASM_SEGMENT_SCOPE:
    throw runtime_error("invalid segment for the thread");
    break;
  case EZ_ASM_SEGMENT_GLOBAL:
    if (addr.offset >= ezMemory::instance().globals().size())
      throw runtime_error("global memory access violation");
    v = ezMemory::instance().globals()[addr.offset];
    break;
  default:
    throw runtime_error("out of segment boundary");
  }
  return v;
}

void ezThread::run(void) {
  if (m_stack.empty())
    return;
  if (m_wait) {
    if (m_callback->exist(m_wait))
      return;
    else
      m_wait = 0;
  }
  switch (m_scheduler) {
  case EZ_THREAD_SCHED_REALTIME:
    do {
      m_stack.back()->step();
      if (m_wait)
        break;
    } while (!m_stack.empty());
    break;
  case EZ_THREAD_SCHED_ROUNDROBIN:
    m_stack.back()->step();
    break;
  }
}

void ezThread::on_mark(void) {
  for (vector<ezStackFrame *>::iterator it = m_stack.begin();
       it != m_stack.end(); it++) {
    (*it)->mark();
    (*it)->on_mark();
  }
}

void ezThread::call(ezStackFrame *sf) {
  ezLog &log = ezLog::instance();
  if (!sf)
    throw("invalid call stack");
  m_stack.push_back(sf);
  log.verbose("stack %p has turn", sf);
}

void ezThread::end(vector<ezAddress> &dests, vector<ezValue*> &vals) {
  ezLog &log = ezLog::instance();
  ezStackFrame *callee = m_stack.back();
  log.verbose("stack %p has poped out", callee);
  m_stack.pop_back();
  if (m_stack.empty()) return;
  ezStackFrame *caller = m_stack.back();
  caller->update(dests, vals);
}

size_t ezThread::thd(ezAddress &func, vector<ezValue*> &args,
                     vector<ezAddress> &rets, ezStackFrame* caller) {
  return m_callback->thd(func, args, rets, caller);
}

void ezThread::wait(size_t handle) { m_wait = handle; }
