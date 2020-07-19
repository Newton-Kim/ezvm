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

#include "ezvm/asm/ezasm_proc.h"
#include "ezvm/ezstack.h"

using namespace std;

ezAsmProcedure::ezAsmProcedure(ezFunction *carousel, ezAsmProcedure *parent)
    : m_carousel(carousel), m_local_index(0), m_scope(NULL) {
  if (parent) m_scope = &parent->m_locals;
}

void ezAsmProcedure::args(size_t args) { m_carousel->nargs = args; }

void ezAsmProcedure::mems(size_t mems) { m_carousel->nmems = mems; }

void ezAsmProcedure::temps(size_t temps) { m_carousel->ntemps = temps; }

size_t ezAsmProcedure::local(const string value) {
  if (m_locals.end() == m_locals.find(value))
    m_locals[value] = m_local_index++;
  if (m_carousel->nmems < m_local_index)
    m_carousel->nmems = m_local_index;
  return m_locals[value];
}

bool ezAsmProcedure::search_unresolved(string label) {
  bool found = false;
  for (list<string>::iterator it = m_unresolved.begin();
       it != m_unresolved.end(); it++) {
    if (*it == label) {
      found = true;
      break;
    }
  }
  return found;
}

size_t ezAsmProcedure::label2index(string label) {
  if (!m_carousel->jmptbl.exist(label)) {
    if (!search_unresolved(label))
      m_unresolved.push_back(label);
    m_carousel->jmptbl.add(label, 0);
  }
  return m_carousel->jmptbl[label];
}

void ezAsmProcedure::label(string name, size_t offset) {
  if (search_unresolved(name))
    m_unresolved.remove(name);
  m_carousel->jmptbl.add(name, m_carousel->instruction.size() + offset);
}

void ezAsmProcedure::append_instruction(ezAsmInstruction *instr) {
  m_carousel->instruction.insert(m_carousel->instruction.end(),
                                 instr->m_instruction.begin(),
                                 instr->m_instruction.end());
  instr->m_instruction.clear();
}

void ezAsmProcedure::validate(void) {
  if (m_unresolved.size())
    throw runtime_error("unresolved label exists");
}
