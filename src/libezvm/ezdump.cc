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
#include <iostream>

using namespace std;

ezDump::ezDump(ezAddress &entry, vector<ezValue *> &constants,
               ezTable<string, ezValue *> &globals)
    : m_entry(entry), m_constants(constants), m_globals(globals) {}

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
    sink.print("null\n");
    return;
  }
  switch (v->type) {
  case EZ_VALUE_TYPE_NULL:
    sink.print("(nil)");
    break;
  case EZ_VALUE_TYPE_BOOL:
    if (((ezBool *)v)->to_bool() == true)
      sink.print("true");
    else
      sink.print("false");
    break;
  case EZ_VALUE_TYPE_INTEGER:
    sink.print("%d", ((ezInteger *)v)->to_integer());
    break;
  case EZ_VALUE_TYPE_COMPLEX: {
    double cr = ((ezComplex *)v)->to_complex().real();
    double ci = ((ezComplex *)v)->to_complex().imag();
    sink.print("%f %s %fj", cr, (ci > 0 ? "+" : "-"), ci);
  } break;
  case EZ_VALUE_TYPE_STRING:
    sink.print("\"%s\"", ((ezString *)v)->to_string().c_str());
    break;
  case EZ_VALUE_TYPE_CAROUSEL: {
    ezCarousel *crsl = (ezCarousel *)v;
    sink.print("(%d)\n", crsl->nargs);
    sink.print("      .memsize: %lu\n", crsl->nmems);
    sink.print("      .jump table:\n");
    for (size_t i = 0; i < crsl->jmptbl.size(); i++)
      sink.print("        [%lu]=%lu\n", i, crsl->jmptbl[i]);
    sink.print("      .jump symbol table:\n");
    for (map<string, size_t>::iterator it = crsl->local_symtab().begin();
         it != crsl->local_symtab().end(); it++)
      sink.print("        [%s]=%lu\n", it->first.c_str(), it->second);
    ezAddress addr;
    ezOpCode op;
    uint8_t arg[3];
    size_t len = crsl->instruction.size();
    for (size_t pc = 0; pc < len; pc++) {
      sink.print("      %d:", pc);
      crsl->instruction[pc]->dump(sink, *this);
    }
  } break;
  case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
    sink.print("(native)");
    break;
  }
  sink.print("\n");
}

void ezDump::dump(const string path) {
  ezFile sink(path, "wb");
  sink.print(".entry: ");
  dump(sink, m_entry);
  sink.print("\n");
  sink.print(".global:\n");
  sink.indent();
  sink.indentation();
  sink.print("memory:\n");
  sink.indent();
  for (size_t i = 0; i < m_globals.m_memory.size(); i++) {
    sink.indentation();
    sink.print("[%lu]=", i);
    dump(sink, m_globals.m_memory[i]);
  }
  sink.unindent();
  sink.indentation();
  sink.print("symtab:\n");
  sink.indent();
  for (map<string, size_t>::iterator it = m_globals.m_symtab.begin();
       it != m_globals.m_symtab.end(); it++) {
    sink.indentation();
    sink.print("[_%s]=%lu\n", it->first.c_str(), it->second);
  }
  sink.unindent();
  sink.unindent();
  sink.print("\n");
  sink.print(".constant:\n");
  sink.indent();
  for (size_t i = 0; i < m_constants.size(); i++) {
    sink.indentation();
    sink.print("[%lu]=", i);
    dump(sink, m_constants[i]);
  }
  sink.unindent();
  sink.print("\n");
}

const char *ezDump::opstr(ezOpCode op) {
  switch (op) {
  case EZ_OP_ADD:
    return "add";
  case EZ_OP_AND:
    return "add";
  case EZ_OP_BEQ:
    return "beq";
  case EZ_OP_BGE:
    return "bge";
  case EZ_OP_BLT:
    return "blt";
  case EZ_OP_BNE:
    return "bne";
  case EZ_OP_BRA:
    return "bra";
  case EZ_OP_CALL:
    return "call";
  case EZ_OP_CMP:
    return "cmp";
  case EZ_OP_DIV:
    return "div";
  case EZ_OP_FGC:
    return "fgc";
  case EZ_OP_LSL:
    return "lsl";
  case EZ_OP_LSR:
    return "lsr";
  case EZ_OP_MOD:
    return "mod";
  case EZ_OP_MUL:
    return "mul";
  case EZ_OP_MV:
    return "mv";
  case EZ_OP_NEG:
    return "neg";
  case EZ_OP_NOT:
    return "not";
  case EZ_OP_OR:
    return "or";
  case EZ_OP_RET:
    return "ret";
  case EZ_OP_SUB:
    return "sub";
  case EZ_OP_TEQ:
    return "teq";
  case EZ_OP_TGE:
    return "tge";
  case EZ_OP_THD:
    return "thd";
  case EZ_OP_TLT:
    return "tlt";
  case EZ_OP_TNE:
    return "tne";
  case EZ_OP_XOR:
    return "xor";
  }
  return "unknown";
}

void ezDump::dump(ezFile &sink, ezOpCode op, ezAddress &func,
                  vector<ezAddress> &srcs, vector<ezAddress> &dests) {
  sink.print("%s", opstr(op));
  dump(sink, func);
  sink.print(",");
  if (srcs.empty())
    sink.print("null");
  else
    for (vector<ezAddress>::iterator it = srcs.begin(); it != srcs.end(); it++)
      dump(sink, *it);
  sink.print(",");
  if (dests.empty())
    sink.print("null");
  else
    for (vector<ezAddress>::iterator it = dests.begin(); it != dests.end();
         it++)
      dump(sink, *it);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, ezOpCode op, ezAddress &func,
                  vector<ezAddress> &srcs, vector<ezAddress> &dests,
                  ezAddress &handle) {
  sink.print("%s", opstr(op));
  dump(sink, func);
  sink.print(",");
  if (srcs.empty())
    sink.print("null");
  else
    for (vector<ezAddress>::iterator it = srcs.begin(); it != srcs.end(); it++)
      dump(sink, *it);
  sink.print(",");
  if (dests.empty())
    sink.print("null");
  else
    for (vector<ezAddress>::iterator it = dests.begin(); it != dests.end();
         it++)
      dump(sink, *it);
  sink.print(",");
  dump(sink, handle);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, ezOpCode op, vector<ezAddress> &result,
                  vector<ezAddress> &param) {
  sink.print("%s", opstr(op));
  if (result.empty())
    sink.print("null");
  else
    for (vector<ezAddress>::iterator it = result.begin(); it != result.end();
         it++)
      dump(sink, *it);
  sink.print(",");
  if (param.empty())
    sink.print("null");
  else
    for (vector<ezAddress>::iterator it = param.begin(); it != param.end();
         it++)
      dump(sink, *it);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, ezOpCode op, vector<ezAddress> &param) {
  sink.print("%s", opstr(op));
  if (param.empty())
    sink.print("null");
  else
    for (vector<ezAddress>::iterator it = param.begin(); it != param.end();
         it++)
      dump(sink, *it);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, ezOpCode op, ezAddress cond, size_t offset) {
  sink.print("%s", opstr(op));
  dump(sink, cond);
  sink.print(",");
  sink.print(" %d", offset);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, ezOpCode op, ezAddress handle) {
  sink.print("%s", opstr(op));
  dump(sink, handle);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, ezOpCode op, size_t offset) {
  sink.print("%s", opstr(op));
  sink.print(" %d", offset);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, ezOpCode op) { sink.print("%s\n", opstr(op)); }
