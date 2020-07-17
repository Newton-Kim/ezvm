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
#include "ezvm/ezfunc.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

ezFunction::ezFunction(bool scope)
    : ezObject(EZ_OBJECT_TYPE_FUNCTION), nargs(0), nmems(0), ntemps(0),
      m_scope(scope) {
  m_size = sizeof(*this);
}
ezFunction::~ezFunction() {
  for (vector<ezInstruction *>::iterator it = instruction.begin();
       it != instruction.end(); it++)
    delete *it;
}
ezUserDefinedFunction::ezUserDefinedFunction()
    : ezObject(EZ_OBJECT_TYPE_USER_DEFINED_FUNCTION) {
  m_size = sizeof(*this);
}
void ezFunction::on_mark(void) {}

/*
map<string, size_t> &ezFunction::local_symtab(void) {
  return (m_local) ? m_local->symtab() : m_symtab;
}
*/

void ezFunction::dump(ezFile &sink) {
  sink.print("0x%x(%d)\n", this, nargs);
  sink.print("  .scoped: %s\n", m_scope ? "true" : "false");
  sink.print("  .memsize: %lu\n", nmems);
  sink.print("  .jump table:\n");
  for (size_t i = 0; i < jmptbl.size(); i++)
    sink.print("    [%lu]=%lu\n", i, jmptbl[i]);
  sink.print("  .jump symbol table:\n");
  vector<string> symbols;
  jmptbl.symbols(symbols);
  for (vector<string>::iterator it = symbols.begin(); it != symbols.end(); it++)
    sink.print("    [%s]=%lu\n", (*it).c_str(), jmptbl[*it]);
  ezAddress addr;
  string op;
  uint8_t arg[3];
  size_t len = instruction.size();
  for (size_t pc = 0; pc < len; pc++) {
    sink.print("  %d:", pc);
    instruction[pc]->dump(sink);
  }
}
