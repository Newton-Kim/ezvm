#include "ezinstruction.h"

struct ezOperation{
	uint8_t code;
	uint8_t arg1;
	uint8_t arg2;
	uint8_t arg3;
};
struct ezArgument{
	uint8_t segment;
	unsigned int offset:24;
};

ezInstEncoder::ezInstEncoder(vector<ezInstruction>& instr): m_instruction(instr) {
}

void ezInstEncoder::opcode(ezOpCode op, uint8_t arg1, uint8_t arg2, uint8_t arg3){
	struct ezOperation opc;
	opc.code = op;
	opc.arg1 = arg1;
	opc.arg2 = arg2;
	opc.arg3 = arg3;
	m_instruction.push_back(*((ezInstruction*)&opc));
}

void ezInstEncoder::argument(ezAddress addr){
	struct ezArgument arg;
	arg.segment = addr.segment;
	arg.offset = addr.offset;
	m_instruction.push_back(*((ezInstruction*)&arg));
}

