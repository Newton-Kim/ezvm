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
  ezStepState state = sf->step();
  switch(state) {
    case EZ_STEP_DONE:
      log.verbose("stack %p has poped out", sf);
      m_stack.pop_back();
      break;
    case EZ_STEP_CALL:
      {
        ezStackFrame* nsf = sf->callee();
        if(!nsf) throw("invalid call stack");
        m_stack.push_back(nsf);
      }
      break;
  }
  log.verbose("stack %p has turn", sf);
  return EZ_STEP_CONTINUE;
}

ezValue *ezThread::addr2val(ezAddress addr) {
  ezValue *v = NULL;
  switch (addr.segment) {
  case EZ_ASM_SEGMENT_CONSTANT:
    if (addr.offset >= m_constants.size())
      throw runtime_error("constant memory access violation");
    v = m_constants[addr.offset];
    break;
  case EZ_ASM_SEGMENT_LOCAL:
  case EZ_ASM_SEGMENT_SCOPE:
    throw runtime_error("invalid segment for the thread");
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

void ezThread::on_mark(void) {
  for (vector<ezStackFrame *>::iterator it = m_stack.begin();
       it != m_stack.end(); it++) {
    (*it)->on_mark();
  }
}
