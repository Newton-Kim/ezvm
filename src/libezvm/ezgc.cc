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

ezGC::ezGC() : m_size(0), m_prev_size(0), m_pause(false) {}

ezGC::~ezGC() {}

void ezGC::collect(void) {
  for (typename vector<ezGCClient *>::iterator it = m_clients.begin();
       it != m_clients.end(); it++) {
    (*it)->on_mark();
  }

  typename list<ezGCObject *>::iterator it = m_memories.begin();
  while(it != m_memories.end()) {
    ezGCObject *value = *it;
    if (value->marked()) {
      value->unmark();
      it++;
    } else {
      m_size -= value->size();
      it = m_memories.erase(it);
      delete value;
    }
  }
  m_prev_size = m_size;
}

void ezGC::add(ezGCObject *v) {
  if (m_pause)
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
  static ezGC* s_ezGC = new ezGC;
  return *s_ezGC;
}
