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
#include "ezvm/asm/ezasm.h"
#include "ezvm/ezmemory.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

ezASM::ezASM(ezAddress &entry)
    : m_entry(entry), m_constants(ezMemory::instance()->constants()),
      m_globals(ezMemory::instance()->globals()), m_gc(ezGC::instance()) {}

ezASM::~ezASM() {}

void ezASM::load_intrinsics(char **symtab, ezObject **constants) {
  if (!symtab || !constants)
    throw runtime_error("entry is not found");
  map<string, size_t> *offset_symtab = new map<string, size_t>;
  /*TODO:should it be put to constant?*/
  for (size_t i = 0; constants[i] && symtab[i]; i++) {
    size_t offset = m_globals.add(symtab[i], constants[i]);
    m_gc.add((ezGCObject *)constants[i]);
  }
}

void ezASM::entry(const string entry) { m_entry_string = entry; }

void ezASM::reset(const string name) { m_globals.reset(name); }

ezAsmProcedure *ezASM::new_proc(const string name, ezAsmProcedure *parent) {
  ezFunction *carousel = new ezFunction((parent) ? true : false);
  size_t offset = m_globals.add(name, carousel);
  m_gc.add((ezGCObject *)carousel);
  if (name == m_entry_string) {
    m_entry.segment = EZ_ASM_SEGMENT_GLOBAL;
    m_entry.offset = offset;
  }
  return new ezAsmProcedure(carousel, parent);
}

size_t ezASM::global(const string value) {
  //	vector<ezValue*>* offset = m_globals[0];
  if (!m_globals.exist(value))
    m_globals.add(value, ezNull::instance());
  //    throw runtime_error("global symbol " + value + " is not found");
  return m_globals[value];
}

bool ezASM::is_global(const string value) { return m_globals.exist(value); }

size_t ezASM::constant_null(void) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezObject *v = m_constants[i];
    if (v->type == EZ_OBJECT_TYPE_NULL)
      return i;
  }
  size_t idx = m_constants.size();
  ezObject *v = ezNull::instance();
  m_constants.push_back(v);
  return idx;
}

size_t ezASM::constant(ezValue *arg, ezALU *alu) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezObject *v = m_constants[i];
    if (v->type == EZ_OBJECT_TYPE_VALUE && alu->is_equal((ezValue *)v, arg))
      return i;
  }
  size_t idx = m_constants.size();
  m_constants.push_back(arg);
  m_gc.add((ezGCObject *)arg);
  return idx;
}
