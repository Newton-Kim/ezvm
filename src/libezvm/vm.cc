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
#include "ezvm/ezmemory.h"
#include "ezvm/ezvm.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace std;

ezVM::ezVM(ezUsrALU *usr_alu) : m_pasm(NULL), m_parchive(NULL) {
  ezALU::initialize(usr_alu);
  ezGC::instance().subscribe(this);
  ezGC::instance().subscribe(&ezMemory::instance());
}

/** \page thread-model Threading model
* TBD
*/

ezVM::~ezVM() {
  if (m_pasm)
    delete m_pasm;
  if (m_parchive)
    delete m_parchive;

  for (list<ezThread *>::iterator it = m_threads.begin(); it != m_threads.end();
       it++)
    if (*it)
      delete *it;
}

void ezVM::run(void) {
  vector<ezValue*> args;
  vector<ezAddress> rets;
  ezThread *thread = new ezThread(m_entry, args, rets, this);
  m_threads.push_back(thread);
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

ezArchive &ezVM::archive(void) {
  if (!m_parchive)
    m_parchive = new ezArchive();
  return *m_parchive;
}

ezDump &ezVM::dump(void) {
  if (!m_pdump)
    m_pdump = new ezDump(m_entry);
  return *m_pdump;
}

void ezVM::on_mark(void) {
  for (list<ezThread *>::iterator it = m_threads.begin(); it != m_threads.end();
       it++)
    (*it)->on_mark();
}

size_t ezVM::thd(ezAddress &func, vector<ezValue*> &args,
                 vector<ezAddress> &rets, ezStackFrame* caller) {
  ezThread *thread =
      new ezThread(func, args, rets, this, EZ_THREAD_SCHED_ROUNDROBIN, caller);
  m_threads.push_back(thread);
  return (size_t)thread;
}

bool ezVM::exist(size_t handle) {
  auto it = find(m_threads.begin(), m_threads.end(), (ezThread *)handle);
  return it != m_threads.end();
}
