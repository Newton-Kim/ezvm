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
#include "ezinstruction.h"
#include <stdexcept>

struct ezOperation {
  uint8_t code;
  uint8_t arg1;
  uint8_t arg2;
  uint8_t arg3;
};
struct ezArgument {
  int8_t segment;
  unsigned int offset : 24;
};

ezInstEncoder::ezInstEncoder(vector<ezInstruction>& instr)
    : m_instruction(instr) {}

void ezInstEncoder::opcode(ezOpCode op, uint8_t arg1, uint8_t arg2,
                           uint8_t arg3) {
  struct ezOperation opc;
  opc.code = op;
  opc.arg1 = arg1;
  opc.arg2 = arg2;
  opc.arg3 = arg3;
  m_instruction.push_back(*((ezInstruction*)&opc));
}

void ezInstEncoder::argument(ezAddress addr) {
  struct ezArgument arg;
  arg.segment = addr.segment;
  arg.offset = addr.offset;
  m_instruction.push_back(*((ezInstruction*)&arg));
}

void ezInstDecoder::opcode(ezInstruction inst, ezOpCode& op, uint8_t& arg1,
                           uint8_t& arg2, uint8_t& arg3) {
  ezOperation operation = *((ezOperation*)&inst);
  op = (ezOpCode) operation.code;
  arg1 = operation.arg1;
  arg2 = operation.arg2;
  arg3 = operation.arg3;
}

void ezInstDecoder::argument(ezInstruction inst, ezAddress& addr) {
  ezArgument arg = *((ezArgument*)&inst);
  addr.segment = arg.segment;
  addr.offset = arg.offset;
}

const char* ezInstDecoder::opstr(ezOpCode op) {
  switch (op) {
    case EZ_OP_MV:
      return "mv";
    case EZ_OP_LD:
      return "ld";
    case EZ_OP_CALL:
      return "call";
    case EZ_OP_ADD:
      return "add";
    case EZ_OP_SUB:
      return "add";
    case EZ_OP_BEQ:
      return "beq";
    case EZ_OP_BLT:
      return "blt";
    case EZ_OP_BRA:
      return "bra";
  }
  return "unknown";
}
