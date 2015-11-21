#include "ezvm/ezval.h"

ezValue::ezValue(const ezValueType tp, const bool dyn): m_reference(0), type(tp), dynamic(dyn) {}
ezValue::~ezValue(){}
size_t ezValue::reference(void) { m_reference++; return m_reference; }
size_t ezValue::release(void) { if(m_reference > 0) m_reference--; return m_reference; }

ezNull::ezNull():ezValue(EZ_VALUE_TYPE_NULL, false) {}
ezNull* ezNull::instance() {
	static ezNull null;
	return &null;
}

ezBool::ezBool(bool val, const bool dynamic):ezValue(EZ_VALUE_TYPE_BOOL, dynamic), m_value(val) {}
const bool ezBool::value(void) { return m_value; }

ezInteger::ezInteger(int val, const bool dynamic):ezValue(EZ_VALUE_TYPE_INTEGER, dynamic), m_value(val) {}
const int ezInteger::value(void) { return m_value; }

ezString::ezString(const string val, const bool dynamic):ezValue(EZ_VALUE_TYPE_STRING, dynamic), m_value(val) {}
const string ezString::value(void){ return m_value; }

ezCarousel::ezCarousel(uint8_t args, uint8_t rets, size_t mems, const bool dynamic):
	ezValue(EZ_VALUE_TYPE_CAROUSEL, dynamic),
	nargs(args), nrets(rets), nmems(mems){}
ezCarousel::~ezCarousel(){}
ezNativeCarousel::ezNativeCarousel(const bool dynamic): ezValue(EZ_VALUE_TYPE_NATIVE_CAROUSEL, dynamic) {}
