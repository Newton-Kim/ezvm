#pragma once

#include <cstddef>
#include <vector>
#include <string>

using namespace std;

#define EZ_ASM_SEGMENT_CONSTANT	0
#define EZ_ASM_SEGMENT_GLOBAL	1
#define EZ_ASM_SEGMENT_LOCAL	2
#define EZ_ASM_SEGMENT_PARENT	3
#define EZ_ASM_SEGMENT_EXTERNAL	4

class ezAddress {
	public:
		ezAddress():segment(EZ_ASM_SEGMENT_CONSTANT), offset(0) {}
		ezAddress(ezAddress& addr) :segment(addr.segment), offset(addr.offset) {}
		ezAddress(size_t seg, size_t off) :segment(seg), offset(off) {}
		size_t segment;
		size_t offset;
};

class ezAsmProcedure {
	public:
		void call(const ezAddress func, vector<ezAddress>& args, vector<ezAddress>& rets);
		void mv(vector<ezAddress>& dest, vector<ezAddress>& src);
		void ld(const ezAddress dest, const ezAddress obj, const ezAddress offset);
};

class ezASM {
	public:
		void entry(const string name);
		ezAsmProcedure* new_proc(const string name, int argc, int retc);
};
