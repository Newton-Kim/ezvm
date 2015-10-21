#pragma once
#include <cstddef>

#define EZ_ASM_SEGMENT_CONSTANT	0
#define EZ_ASM_SEGMENT_GLOBAL	1
#define EZ_ASM_SEGMENT_LOCAL	2
#define EZ_ASM_SEGMENT_PARENT	3
#define EZ_ASM_SEGMENT_EXTERNAL	4

class ezAddress {
	public:
		ezAddress():segment(EZ_ASM_SEGMENT_CONSTANT), offset(0) {}
		ezAddress(const ezAddress& addr) :segment(addr.segment), offset(addr.offset) {}
		ezAddress(size_t seg, size_t off) :segment(seg), offset(off) {}
		size_t segment;
		size_t offset;
};

