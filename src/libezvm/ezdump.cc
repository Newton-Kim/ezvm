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
    (*it).dump(sink);
}

void ezDump::dump(const string path) {
}

void ezDump::dump(ezFile &sink, string op, ezAddress &func,
                  vector<ezAddress> &srcs, vector<ezAddress> &dests) {
  sink.print("%s", op.c_str());
  func.dump(sink);
  sink.print(",");
  if (srcs.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = srcs.begin(); it != srcs.end(); it++)
      (*it).dump(sink);
  sink.print(",");
  if (dests.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = dests.begin(); it != dests.end();
         it++)
      (*it).dump(sink);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, ezAddress &func,
                  vector<ezAddress> &srcs, vector<ezAddress> &dests,
                  ezAddress &handle) {
  sink.print("%s", op.c_str());
  func.dump(sink);
  sink.print(",");
  if (srcs.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = srcs.begin(); it != srcs.end(); it++)
      (*it).dump(sink);
  sink.print("; ");
  if (dests.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = dests.begin(); it != dests.end();
         it++)
      (*it).dump(sink);
  sink.print(",");
  handle.dump(sink);
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
      (*it).dump(sink);
  sink.print(",");
  if (param.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = param.begin(); it != param.end();
         it++)
      (*it).dump(sink);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, vector<ezAddress> &param) {
  sink.print("%s", op.c_str());
  if (param.empty())
    sink.print(" null");
  else
    for (vector<ezAddress>::iterator it = param.begin(); it != param.end();
         it++)
      (*it).dump(sink);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, ezAddress cond, size_t offset) {
  sink.print("%s", op.c_str());
  cond.dump(sink);
  sink.print(",");
  sink.print(" %d", offset);
  sink.print("\n");
}

void ezDump::dump(ezFile &sink, string op, ezAddress handle) {
  sink.print("%s", op.c_str());
  handle.dump(sink);
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
  dest.dump(sink);
  sink.print(",");
  src.dump(sink);
  sink.print("\n");
}

void ezDump::unary(ezFile &sink, string op, ezAddress dest, ezAddress cond,
                   ezAddress src) {
  sink.print("%s", op.c_str());
  dest.dump(sink);
  cond.dump(sink);
  sink.print(",");
  src.dump(sink);
  sink.print("\n");
}

void ezDump::binary(ezFile &sink, string op, ezAddress dest, ezAddress lsrc,
                    ezAddress rsrc) {
  sink.print("%s", op.c_str());
  dest.dump(sink);
  sink.print(",");
  lsrc.dump(sink);
  rsrc.dump(sink);
  sink.print("\n");
}

void ezDump::binary(ezFile &sink, string op, ezAddress dest, ezAddress cond,
                    ezAddress lsrc, ezAddress rsrc) {
  sink.print("%s", op.c_str());
  dest.dump(sink);
  cond.dump(sink);
  sink.print(",");
  lsrc.dump(sink);
  rsrc.dump(sink);
  sink.print("\n");
}

ezDump* ezDump::instance(void) {
  static ezDump s_dump;
  return &s_dump;
}
