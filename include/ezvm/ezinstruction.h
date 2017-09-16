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
#pragma once

#include "ezvm/ezaddr.h"
#include "ezvm/ezfile.h"
#include <functional>
#include <vector>

using namespace std;

enum ezOpCode {
  EZ_OP_ADD = 0,
  EZ_OP_AND,
  EZ_OP_BEQ,
  EZ_OP_BGE,
  EZ_OP_BLT,
  EZ_OP_BNE,
  EZ_OP_BRA,
  EZ_OP_CALL,
  EZ_OP_CMP,
  EZ_OP_DIV,
  EZ_OP_FGC,
  EZ_OP_LSL,
  EZ_OP_LSR,
  EZ_OP_MOD,
  EZ_OP_MUL,
  EZ_OP_MV,
  EZ_OP_NEG,
  EZ_OP_NOT,
  EZ_OP_OR,
  EZ_OP_POW,
  EZ_OP_RET,
  EZ_OP_SUB,
  EZ_OP_TEQ,
  EZ_OP_TGE,
  EZ_OP_THD,
  EZ_OP_TLT,
  EZ_OP_TNE,
  EZ_OP_WAIT,
  EZ_OP_XOR
};

class ezStackFrame;
class ezDump;

class ezInstruction {
private:
  function<void(ezStackFrame &stk, ezInstruction &arg)> m_func;
  function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> m_dump;

public:
  ezInstruction(
      function<void(ezStackFrame &stk, ezInstruction &arg)> func,
      function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump);
  size_t offset;
  ezAddress arg;
  ezAddress handle;
  vector<ezAddress> srcs;
  vector<ezAddress> dests;
  inline void process(ezStackFrame &stk) { m_func(stk, *this); }
  inline void dump(ezFile &sink, ezDump &dump) { m_dump(sink, dump, *this); }
};
