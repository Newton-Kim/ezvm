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
#include "ezvm/ezval.h"
#include <vector>
#include <cstdint>

using namespace std;

enum ezOpCode {
  EZ_OP_ADD = 0,
  EZ_OP_AND,
  //  EZ_OP_BGE,
  EZ_OP_BEQ,
  //  EZ_OP_BNE,
  EZ_OP_BLT,
  EZ_OP_BRA,
  EZ_OP_CALL,
  //  EZ_OP_CMP,
  EZ_OP_DIV,
  EZ_OP_LD,
  //  EZ_OP_LSL,
  //  EZ_OP_LSR,
  //  EZ_OP_MOD,
  EZ_OP_MUL,
  EZ_OP_MV,
  EZ_OP_NEG,
  //  EZ_OP_NOT,
  EZ_OP_OR,
  //  EZ_OP_RET,
  EZ_OP_SUB,
  EZ_OP_XOR
};

class ezInstEncoder {
 private:
  vector<ezInstruction>& m_instruction;

 public:
  ezInstEncoder(vector<ezInstruction>& instr);
  void opcode(ezOpCode op, uint8_t arg1 = 0, uint8_t arg2 = 0,
              uint8_t arg3 = 0);
  void argument(ezAddress addr);
};

class ezInstDecoder {
 public:
  void opcode(ezInstruction inst, ezOpCode& op, uint8_t& arg1, uint8_t& arg2,
              uint8_t& arg3);
  void argument(ezInstruction inst, ezAddress& addr);
  const char* opstr(ezOpCode op);
};
