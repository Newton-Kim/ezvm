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
#include "ezvm/ezinstruction.h"
#include <stdexcept>
//#include <iostream>

#define INST_MASK 0x7f
#define INST_BIT 0x80

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

ezInstEncoder::ezInstEncoder(vector<ezInstruction *> &instr)
    : m_instruction(instr) {}

void ezInstEncoder::opcode(ezOpCode op, uint8_t arg1, uint8_t arg2,
                           uint8_t arg3) {
  struct ezOperation opc;
  opc.code = op | INST_BIT;
  opc.arg1 = arg1;
  opc.arg2 = arg2;
  opc.arg3 = arg3;
  m_instruction.push_back(*((ezInstruction *)&opc));
  //  cerr << "enc:" << op << "->" << hex << *(ezInstruction *)&opc << dec <<
  //  endl;
}

void ezInstEncoder::argument(ezAddress addr) {
  struct ezArgument arg;
  arg.segment = addr.segment;
  arg.offset = addr.offset;
  m_instruction.push_back(*((ezInstruction *)&arg));
  //  cerr << "enc:" << "arg->" << hex << *(ezInstruction *)&arg << dec << endl;
}

void ezInstDecoder::opcode(ezInstruction inst, ezOpCode &op, uint8_t &arg1,
                           uint8_t &arg2, uint8_t &arg3) {
  ezOperation operation = *((ezOperation *)&inst);
  //  cerr << "dec:" << hex << inst << dec << endl;
  if (!(operation.code & INST_BIT))
    throw runtime_error("not an instruction");
  op = (ezOpCode)(operation.code & INST_MASK);
  arg1 = operation.arg1;
  arg2 = operation.arg2;
  arg3 = operation.arg3;
}

void ezInstDecoder::argument(ezInstruction inst, ezAddress &addr) {
  ezArgument arg = *((ezArgument *)&inst);
  //  cerr << "dec:" << "arg->" << hex << inst << dec << endl;
  if ((arg.segment & INST_BIT) != 0)
    throw runtime_error("not an address");
  addr.segment = arg.segment;
  addr.offset = arg.offset;
}

const char *ezInstDecoder::opstr(ezOpCode op) {
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
  case EZ_OP_SUB:
    return "sub";
  case EZ_OP_XOR:
    return "xor";
  }
  return "unknown";
}
