#pragma once

#include "ezvm/ezaddr.h"
#include "ezvm/ezval.h"
#include <vector>
#include <cstdint>

using namespace std;

enum ezOpCode {
	EZ_OP_MV = 0,
	EZ_OP_LD,
	EZ_OP_CALL
};

class ezInstEncoder {
	private:
		vector<ezInstruction>& m_instruction;
	public:
		ezInstEncoder(vector<ezInstruction>& instr);
		void opcode(ezOpCode op, uint8_t arg1 = 0, uint8_t arg2 = 0, uint8_t arg3 = 0);
		void argument(ezAddress addr);
};

class ezInstDecoder {
	public:
		void opcode(ezInstruction inst, ezOpCode& op, uint8_t& arg1, uint8_t& arg2, uint8_t& arg3);
};
