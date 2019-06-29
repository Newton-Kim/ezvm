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

ezDump::ezDump() {}

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
  case EZ_ASM_SEGMENT_TEMPORARY:
    sink.print(" t");
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

void ezDump::dump(ezFile &sink, ezObject *o) {
  if (!o) {
    sink.print(" (NULL)\n");
    return;
  }
  o->dump(sink);
}

void ezDump::dump(const string path) {
  ezFile sink(path, "wb");
/*TODO: send it to the ezVM::dump
  sink.print(".entry: ");
  dump(sink, m_entry);
*/
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

void ezDump::unary(ezFile &sink, string op, ezAddress dest, ezAddress src) {
  sink.print("%s", op.c_str());
  dump(sink, dest);
  sink.print(",");
  dump(sink, src);
  sink.print("\n");
}

void ezDump::unary(ezFile &sink, string op, ezAddress dest, ezAddress cond,
                   ezAddress src) {
  sink.print("%s", op.c_str());
  dump(sink, dest);
  dump(sink, cond);
  sink.print(",");
  dump(sink, src);
  sink.print("\n");
}

void ezDump::binary(ezFile &sink, string op, ezAddress dest, ezAddress lsrc,
                    ezAddress rsrc) {
  sink.print("%s", op.c_str());
  dump(sink, dest);
  sink.print(",");
  dump(sink, lsrc);
  dump(sink, rsrc);
  sink.print("\n");
}

void ezDump::binary(ezFile &sink, string op, ezAddress dest, ezAddress cond,
                    ezAddress lsrc, ezAddress rsrc) {
  sink.print("%s", op.c_str());
  dump(sink, dest);
  dump(sink, cond);
  sink.print(",");
  dump(sink, lsrc);
  dump(sink, rsrc);
  sink.print("\n");
}

ezDump* ezDump::instance(void) {
  static ezDump s_dump;
  return &s_dump;
}
