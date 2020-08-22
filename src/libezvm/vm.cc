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
#include "ezvm/ezlog.h"
#include "ezvm/ezmemory.h"
#include "ezvm/ezvm.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace std;

ezVM::ezVM() : m_pasm(NULL) {
  ezGC::instance().subscribe(this);
  ezGC::instance().subscribe(&ezMemory::instance());
}

/** \page thread-model Threading model
 * TBD
 */

ezVM::~ezVM() {
  if (m_pasm)
    delete m_pasm;

  for (list<ezThread *>::iterator it = m_threads.begin(); it != m_threads.end();
       it++)
    if (*it)
      delete *it;
}

void ezVM::run(void) {
  vector<ezObject *> args;
  ezAddress ret;
  ezGC::instance().pause();
  ezThread *thread = new ezThread(m_entry, args, ret, this);
  m_threads.push_back(thread);
  ezGC::instance().resume();
  while (!m_threads.empty()) {
    list<ezThread *>::iterator it = m_threads.begin();
    while (it != m_threads.end()) {
      ezThread *thd = *it;
      if (thd == NULL) {
        it = m_threads.erase(it);
      } else {
        thd->run();
        if (thd->empty()) {
          it = m_threads.erase(it);
          delete thd;
        }
      }
      it++;
    }
  }
}

ezASM &ezVM::assembler(void) {
  if (!m_pasm)
    m_pasm = new ezASM(m_entry);
  return *m_pasm;
}

void ezVM::on_mark(void) {
  for (list<ezThread *>::iterator it = m_threads.begin(); it != m_threads.end();
       it++)
    (*it)->on_mark();
}

size_t ezVM::thd(ezAddress &func, vector<ezObject *> &args, ezAddress &ret,
                 ezStackFrame *caller) {
  ezGC::instance().pause();
  ezThread *thread =
      new ezThread(func, args, ret, this, EZ_THREAD_SCHED_ROUNDROBIN, caller);
  m_threads.push_back(thread);
  ezGC::instance().resume();
  return (size_t)thread;
}

size_t ezVM::thd(ezAddress &func, vector<ezObject *> &args,
                 ezStackFrame *caller) {
  ezGC::instance().pause();
  ezThread *thread =
      new ezThread(func, args, this, EZ_THREAD_SCHED_ROUNDROBIN, caller);
  m_threads.push_back(thread);
  ezGC::instance().resume();
  return (size_t)thread;
}

bool ezVM::exist(size_t handle) {
  auto it = find(m_threads.begin(), m_threads.end(), (ezThread *)handle);
  return it != m_threads.end();
}

void ezVM::dump(string path) {
  EZ_INFO("start");
  ezFile sink(path, "wb");
  sink.print(".entry: ");
  m_entry.dump(sink);
  sink.print("\n");
  sink.print("\n.global memory:\n");
  ezTable<string, ezObject *> globals = ezMemory::instance().globals();
  size_t sz = globals.size();
  sink.print("  .size:%d\n", sz);
  for (size_t i = 0; i < sz; i++) {
    sink.print("[%lu]=", i);
    if (globals[i])
      globals[i]->dump(sink);
    else
      sink.print("null\n");
  }
  sink.print("\n");
  sink.print(".global symtab:\n");
  vector<string> symbols;
  ezMemory::instance().globals().symbols(symbols);
  for (vector<string>::iterator it = symbols.begin(); it != symbols.end();
       it++) {
    sink.print("[_%s]=%lu\n", (*it).c_str(),
               ezMemory::instance().globals()[*it]);
  }
  sink.print("\n");
  sink.print(".constant:\n");
  for (size_t i = 0; i < ezMemory::instance().constants().size(); i++) {
    sink.print("[%lu]=", i);
    sink, ezMemory::instance().constants()[i]->dump(sink);
  }
  sink.print("\n");
  sink.print(".call stack:\n");
  for (list<ezThread *>::iterator it = m_threads.begin(); it != m_threads.end();
       it++)
    (*it)->dump(sink);
  EZ_INFO("end");
}
