#pragma once
#include <cstddef>

enum ezValueType {
	EZ_VALUE_TYPE_INTEGER,
	EZ_VALUE_TYPE_STRING,
	EZ_VALUE_TYPE_CAROUSEL,
	EZ_VALUE_TYPE_NATIVE_CAROUSEL,
	EZ_VALUE_TYPE_MAX
};

class ezValue {
	private:
		size_t m_reference;
	public:
		const ezValueType type;
		ezValue(ezValueType tp): type(tp) {};
		virtual ~ezValue(){};
		void reference(void) { m_reference++; }
		void release(void) { m_reference--; }
};
