#pragma once
#include <cstddef>

#define EZ_ASM_SEGMENT_CONSTANT	(-1)
#define EZ_ASM_SEGMENT_LOCAL	(-2)
#define EZ_ASM_SEGMENT_PARENT	(-3)
#define EZ_ASM_SEGMENT_GLOBAL	(0)

class ezAddress {
	public:
		ezAddress():segment(EZ_ASM_SEGMENT_CONSTANT), offset(0) {}
		ezAddress(const ezAddress& addr) :segment(addr.segment), offset(addr.offset) {}
		ezAddress(signed char seg, size_t off) :segment(seg), offset(off) {}
		signed char segment;
		size_t offset;
};

