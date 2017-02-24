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
#include "ezvm/ezlog.h"
#include "ezvm/ezvm.h"
#include <iostream>
#include <stdexcept>

using namespace std;

ezVM::ezVM(ezUsrALU *usr_alu) : m_pasm(NULL), m_parchive(NULL), m_alu(m_gc, usr_alu) {
  m_gc.subscribe(this);
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
  ezLog &log = ezLog::instance();
  log.verbose("%s", __PRETTY_FUNCTION__);
  ezThread *thread = new ezThread(m_entry, m_globals, m_constants, m_alu, m_gc);
  m_threads.push_back(thread);
  log.debug("m_threads is %lu", m_threads.size());
  while (!m_threads.empty()) {
    list<ezThread *>::iterator it = m_threads.begin();
    while (it != m_threads.end()) {
      ezThread *thd = *it;
      log.verbose("m_thread(%p) gets turn", thd);
      if (thd == NULL) {
        it = m_threads.erase(it);
      } else {
        switch (thd->step()) {
        case EZ_STEP_DONE:
          it = m_threads.erase(it);
          delete thd;
          break;
        default:
          it++;
          break;
        }
      }
    }
  }
}

ezASM &ezVM::assembler(void) {
  if (!m_pasm)
    m_pasm = new ezASM(m_entry, m_constants, m_globals, m_gc);
  return *m_pasm;
}

ezArchive &ezVM::archive(void) {
  if (!m_parchive)
    m_parchive = new ezArchive();
  return *m_parchive;
}

ezDump &ezVM::dump(void) {
  if (!m_pdump)
    m_pdump = new ezDump(m_entry, m_constants, m_globals);
  return *m_pdump;
}

void ezVM::on_mark(void) {
  for (vector<ezValue *>::iterator it = m_globals.m_memory.begin();
       it != m_globals.m_memory.end(); it++) {
    (*it)->mark();
    if ((*it)->type == EZ_VALUE_TYPE_CAROUSEL)
      ((ezCarousel *)(*it))->on_mark();
  }
  for (vector<ezValue *>::iterator it = m_constants.begin();
       it != m_constants.end(); it++)
    (*it)->mark();
  for (list<ezThread *>::iterator it = m_threads.begin(); it != m_threads.end();
       it++)
    (*it)->on_mark();
}
