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
#include "ezvm/ezmemory.h"
#include "ezvm/ezfunc.h"

class ezMemoryImpl : public ezMemory {
private:
  ezTable<string, ezObject *> m_globals;
  vector<ezObject *> m_constants;

protected:
  void on_mark(void);

public:
  ezMemoryImpl();
  ezTable<string, ezObject *> &globals(void);
  vector<ezObject *> &constants(void);
};

ezMemoryImpl::ezMemoryImpl() : m_globals(ezNull::instance()) {}

ezTable<string, ezObject *> &ezMemoryImpl::globals(void) { return m_globals; }

vector<ezObject *> &ezMemoryImpl::constants(void) { return m_constants; }

void ezMemoryImpl::on_mark(void) {
  for (vector<ezObject *>::iterator it = m_globals.to_vector().begin();
       it != m_globals.to_vector().end(); it++) {
    if (*it == NULL)
      continue;
    (*it)->mark();
    if ((*it)->type == EZ_OBJECT_TYPE_FUNCTION)
      ((ezFunction *)(*it))->mark();
  }
  for (vector<ezObject *>::iterator it = m_constants.begin();
       it != m_constants.end(); it++)
    (*it)->mark();
}

ezMemory *ezMemory::instance(void) {
  static ezMemoryImpl s_memory;
  return &s_memory;
}
