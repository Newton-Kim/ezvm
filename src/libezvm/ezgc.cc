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
#include <iostream>

ezGC::ezGC() : m_size(0), m_prev_size(0), m_pause_count(0) {}

ezGC::~ezGC() {}

void ezGC::collect(void) {
  if (m_pause_count > 0)
    return;
  for (typename vector<ezGCClient *>::iterator it = m_clients.begin();
       it != m_clients.end(); it++) {
    (*it)->on_mark();
  }

  typename list<ezGCObject *>::iterator it = m_memories.begin();
  while (it != m_memories.end()) {
    ezGCObject *value = *it;
    if (value->is_marked()) {
      value->unmark();
      it++;
    } else {
      typename list<ezGCObject *>::iterator oit = it;
      it++;
      m_size -= value->size();
      m_memories.erase(oit);
      delete value;
    }
  }
  m_prev_size = m_size;
}

void ezGC::add(ezGCObject *v) {
  if (!v->is_gray())
    return;
  v->unmark();
  m_size += v->size();
  m_memories.push_back(v);
  if (m_size > m_prev_size * 2 && m_size > EZGC_THRESHOLD)
    collect();
}

void ezGC::subscribe(ezGCClient *t) { m_clients.push_back(t); }

void ezGC::force(void) { collect(); }

ezGC &ezGC::instance(void) {
  static ezGC *s_ezGC = new ezGC;
  return *s_ezGC;
}

void ezGC::pause(void) { m_pause_count++; }

void ezGC::resume(void) {
  if (m_pause_count > 0)
    m_pause_count--;
  if (!m_pause_count)
    collect();
}
