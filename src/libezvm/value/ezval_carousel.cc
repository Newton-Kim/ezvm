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
#include "ezvm/ezval.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

ezFunction::ezFunction(ezTable<string, ezValue *> *local,
                       ezTable<string, ezValue *> *scope)
    : ezValue(EZ_VALUE_TYPE_FUNCTION), nargs(0), nmems(0), m_local(local),
      m_scope(scope) {
  m_size = sizeof(*this);
}
ezFunction::~ezFunction() {
  for (vector<ezInstruction *>::iterator it = instruction.begin();
       it != instruction.end(); it++)
    delete *it;
}
ezUserDefinedFunction::ezUserDefinedFunction() : ezValue(EZ_VALUE_TYPE_USER_DEFINED_FUNCTION) {
  m_size = sizeof(*this);
}
void ezFunction::on_mark(void) {
  if (m_scope)
    m_scope->mark();
  if (m_local)
    m_local->mark();
}

vector<ezValue *> *ezFunction::local_memory(void) {
  vector<ezValue *> *ret = NULL;
  size_t memories = (nmems > nargs) ? nmems : nargs;
  if (m_local) {
    if (0 == m_local->size()) {
      // TODO:refactoring is required
      for (size_t i = 0; i < memories; i++)
        m_local->push_back(ezNull::instance()); // TODO:using stl APIs
    }
    ret = &m_local->to_vector();
  } else {
    ret = new vector<ezValue *>;
    for (size_t i = 0; i < memories; i++)
      ret->push_back(ezNull::instance()); // TODO:using stl APIs
  }
  return ret;
}

/*
map<string, size_t> &ezFunction::local_symtab(void) {
  return (m_local) ? m_local->symtab() : m_symtab;
}
*/

vector<ezValue *> *ezFunction::scope_memory(void) {
  return (m_scope) ? &m_scope->to_vector() : NULL;
}
