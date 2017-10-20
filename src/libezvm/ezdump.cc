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
#include "ezvm/ezdump.h"
#include "ezvm/ezinstruction.h"
#include "ezvm/ezmemory.h"
#include <iostream>

using namespace std;

ezDump::ezDump(ezAddress &entry) : m_entry(entry) {}

void ezDump::dump(ezFile &sink, vector<ezAddress> &addrs) {
  if (addrs.size() == 0) {
    sink.print(" null");
    return;
  }
  for (vector<ezAddress>::iterator it = addrs.begin(); it != addrs.end(); it++)
    dump(sink, *it);
}

void ezDump::dump(ezFile &sink, const ezAddress addr) {
  switch (addr.segment) {
  case EZ_ASM_SEGMENT_CONSTANT:
    sink.print(" c");
    break;
  case EZ_ASM_SEGMENT_LOCAL:
    sink.print(" r");
    break;
  case EZ_ASM_SEGMENT_SCOPE:
    sink.print(" s");
    break;
  case EZ_ASM_SEGMENT_GLOBAL:
    sink.print(" g");
    break;
  default:
    sink.print(" %d", addr.segment);
  }
  sink.print("%u", addr.offset);
}

void ezDump::dump(ezFile &sink, const ezValue *v) {
  if (!v) {
    sink.print(" (null)\n");
    return;
  }
  switch (v->type) {
  case EZ_VALUE_TYPE_NULL:
    sink.print("nil\n");
    break;
  case EZ_VALUE_TYPE_BOOL:
    if (((ezBool *)v)->value == true)
      sink.print("true");
    else
      sink.print("false");
    sink.print("\n");
    break;
  case EZ_VALUE_TYPE_INTEGER:
    sink.print("%d\n", ((ezInteger *)v)->value);
    break;
  case EZ_VALUE_TYPE_COMPLEX: {
    double cr = ((ezComplex *)v)->value.real();
    double ci = ((ezComplex *)v)->value.imag();
    sink.print("%f %s %fj\n", cr, (ci > 0 ? "+" : "-"), ci);
  } break;
  case EZ_VALUE_TYPE_STRING:
    sink.print("\"%s\"\n", ((ezString *)v)->value.c_str());
    break;
  case EZ_VALUE_TYPE_FUNCTION: {
    ezFunction *crsl = (ezFunction *)v;
    sink.print("0x%x(%d)\n", crsl, crsl->nargs);
    sink.print("  .memsize: %lu\n", crsl->nmems);
    sink.print("  .jump table:\n");
    for (size_t i = 0; i < crsl->jmptbl.size(); i++)
      sink.print("    [%lu]=%lu\n", i, crsl->jmptbl[i]);
    sink.print("  .jump symbol table:\n");
    vector<string> symbols;
    crsl->jmptbl.symbols(symbols);
    for (vector<string>::iterator it = symbols.begin(); it != symbols.end();
         it++)
      sink.print("    [%s]=%lu\n", (*it).c_str(), crsl->jmptbl[*it]);
    ezAddress addr;
    string op;
    uint8_t arg[3];
    size_t len = crsl->instruction.size();
    for (size_t pc = 0; pc < len; pc++) {
      sink.print("  %d:", pc);
      crsl->instruction[pc]->dump(sink, *this);
    }
  } break;
  case EZ_VALUE_TYPE_USER_DEFINED_FUNCTION:
    sink.print("0x%x(native)\n", v);
    break;
  default:
    sink.print("(unknown)\n");
    break;
  }
}

void ezDump::dump(const string path) {
  ezFile sink(path, "wb");
  sink.print(".entry: ");
  dump(sink, m_entry);
  sink.print("\n");
  sink.print("\n.global memory:\n");
  for (size_t i = 0; i < ezMemory::instance().globals().size(); i++) {
    sink.print("[%lu]=", i);
    dump(sink, ezMemory::instance().globals()[i]);
  }
  sink.print("\n");
  sink.print(".global symtab:\n");
  vector<string> symbols;
  ezMemory::instance().globals().symbols(symbols);
  for (vector<string>::iterator it = symbols.begin(); it != symbols.end();
       it++) {
    sink.print("[_%s]=%lu\n", (*it).c_str(),
               ezMemory::instance().globals()[*it]);
  }
  sink.print("\n");
  sink.print(".constant:\n");
  for (size_t i = 0; i < ezMemory::instance().constants().size(); i++) {
    sink.print("[%lu]=", i);
    dump(sink, ezMemory::instance().constants()[i]);
  }
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, ezAddress &func,
                  vector<ezAddress> &srcs, vector<ezAddress> &dests) {
  sink.print("%s", op.c_str());
  dump(sink, func);
  sink.print(",");
  if (srcs.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = srcs.begin(); it != srcs.end(); it++)
      dump(sink, *it);
  sink.print(",");
  if (dests.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = dests.begin(); it != dests.end();
         it++)
      dump(sink, *it);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, ezAddress &func,
                  vector<ezAddress> &srcs, vector<ezAddress> &dests,
                  ezAddress &handle) {
  sink.print("%s", op.c_str());
  dump(sink, func);
  sink.print(",");
  if (srcs.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = srcs.begin(); it != srcs.end(); it++)
      dump(sink, *it);
  sink.print("; ");
  if (dests.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = dests.begin(); it != dests.end();
         it++)
      dump(sink, *it);
  sink.print(",");
  dump(sink, handle);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, vector<ezAddress> &result,
                  vector<ezAddress> &param) {
  sink.print("%s", op.c_str());
  if (result.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = result.begin(); it != result.end();
         it++)
      dump(sink, *it);
  sink.print(",");
  if (param.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = param.begin(); it != param.end();
         it++)
      dump(sink, *it);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, vector<ezAddress> &param) {
  sink.print("%s", op.c_str());
  if (param.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = param.begin(); it != param.end();
         it++)
      dump(sink, *it);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, ezAddress cond, size_t offset) {
  sink.print("%s", op.c_str());
  dump(sink, cond);
  sink.print(",");
  sink.print(" %d", offset);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, ezAddress handle) {
  sink.print("%s", op.c_str());
  dump(sink, handle);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, size_t offset) {
  sink.print("%s", op.c_str());
  sink.print(" %d", offset);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op) { sink.print("%s\n", op.c_str()); }
