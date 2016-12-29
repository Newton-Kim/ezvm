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
#pragma once
#include <vector>
#include <list>
#define EZGC_THRESHOLD  (20 * 1024)

using namespace std;

template < class V, class T> class ezGC {
 private:
  vector<T*> m_clients;
  list<V*> m_memories;
  size_t m_size;
  size_t m_prev_size;
  void collect(void);

 public:
  ezGC();
  ~ezGC();
  void add(V* v);
  void subscribe(T* t);
};

template < class V, class T> ezGC<V,T>::ezGC() : m_size(0), m_prev_size(0) {
}

template < class V, class T> ezGC<V,T>::~ezGC() {
}

template < class V, class T> void ezGC<V,T>::collect(void) {
  for(typename vector<T*>::iterator it = m_clients.begin() ; it != m_clients.end() ; it++) {
    (*it)->on_mark();
  }

  for(typename list<V*>::iterator it = m_memories.begin() ; it != m_memories.end() ; it++) {
    V* value = *it;
    if(value->marked()) {
      value->unmark();
    } else {
      m_size -= value->size();
      delete value;
      it = m_memories.erase(it);
    }
  }
  m_prev_size = m_size;
}

template < class V, class T> void ezGC<V,T>::add(V* v) {
  v->unmark();
  m_size += v->size();
  m_memories.push_back(v);
  if(m_size > m_prev_size * 2 && m_size > EZGC_THRESHOLD) collect();
}

template < class V, class T> void ezGC<V,T>::subscribe(T* t) {
  m_clients.push_back(t);
} 
