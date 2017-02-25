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

using namespace std;

ezDump::ezDump(ezAddress &entry, vector<ezValue *> &constants,
               ezTable<string, ezValue *> &globals)
    : m_entry(entry), m_constants(constants), m_globals(globals) {}

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
  case EZ_VALUE_TYPE_COMPLEX:
    {
      double cr = ((ezComplex *)v)->to_complex().real();
      double ci = ((ezComplex *)v)->to_complex().imag();
      sink.print("%f %s %fj", cr, (ci > 0 ? "+":"-"), ci);
    }
    break;
  case EZ_VALUE_TYPE_STRING:
    sink.print("\"%s\"", ((ezString *)v)->to_string().c_str());
    break;
  case EZ_VALUE_TYPE_CAROUSEL: {
    ezCarousel *crsl = (ezCarousel *)v;
    sink.print("(%d)\n", crsl->nargs);
    sink.print("      .memsize: %lu\n", crsl->nmems);
    sink.print("      .jump table:\n");
    for (size_t i = 0; i < crsl->jmptbl.size(); i++)
      sink.print("        [%lu]:%lu\n", i, crsl->jmptbl[i]);
    sink.print("      .jump symbol table:\n");
    for (map<string, size_t>::iterator it = crsl->local_symtab().begin();
         it != crsl->local_symtab().end(); it++)
      sink.print("        %s->%lu\n", it->first.c_str(), it->second);
    ezInstDecoder decoder;
    ezAddress addr;
    ezOpCode op;
    uint8_t arg[3];
    size_t len = crsl->instruction.size();
    size_t pc = 0;
    while (pc < len) {
      decoder.opcode(crsl->instruction[pc++], op, arg[0], arg[1], arg[2]);
      sink.print("      %s", decoder.opstr(op));
      sink.print("(%d:%d:%d)", arg[0], arg[1], arg[2]);
      switch (op) {
      case EZ_OP_BEQ:
      case EZ_OP_BGE:
      case EZ_OP_BLT:
      case EZ_OP_BNE:
        decoder.argument(crsl->instruction[pc++], addr);
        dump(sink, addr);
        sink.print("\n");
        continue;
      case EZ_OP_BRA:
        sink.print("\n");
        continue;
      case EZ_OP_CALL:
        decoder.argument(crsl->instruction[pc++], addr);
        dump(sink, addr);
        break;
      }
      for (size_t c = 0; c < 3; c++) {
        if (!arg[c])
          continue;
        sink.print(",");
        for (size_t i = 0; i < arg[c]; i++) {
          decoder.argument(crsl->instruction[pc++], addr);
          dump(sink, addr);
        }
      }
      sink.print("\n");
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
