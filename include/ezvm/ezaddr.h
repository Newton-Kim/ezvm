#pragma once
#include <cstddef>
#include <cstdint>

#define EZ_ASM_SEGMENT_CONSTANT	((int8_t)-1)
#define EZ_ASM_SEGMENT_LOCAL	((int8_t)-2)
#define EZ_ASM_SEGMENT_PARENT	((int8_t)-3)
#define EZ_ASM_SEGMENT_GLOBAL	((int8_t)0)

class ezAddress {
	public:
		ezAddress():segment(EZ_ASM_SEGMENT_CONSTANT), offset(0) {}
		ezAddress(const ezAddress& addr) :segment(addr.segment), offset(addr.offset) {}
		ezAddress(int8_t seg, size_t off) :segment(seg), offset(off) {}
		int8_t segment;
		size_t offset;
};

