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

#include "ezgc.h"
#include <map>
#include <vector>

using namespace std;

template <class K, class V> class ezTable : public ezGCObject {
public:
  map<K, size_t> m_symtab;
  vector<V> m_memory;
  size_t add(K key, V value);
  bool exist(K key);
};

template <class K, class V> size_t ezTable<K, V>::add(K key, V value) {
  size_t offset = m_memory.size();
  m_memory.push_back(value);
  m_symtab[key] = offset;
  return offset;
}

template <class K, class V> bool ezTable<K, V>::exist(K key) {
  typename map<K, size_t>::iterator it = m_symtab.find(key);
  return (it != m_symtab.end());
}
