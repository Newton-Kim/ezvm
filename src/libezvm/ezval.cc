#include "ezvm/ezval.h"

ezValue::ezValue(const ezValueType tp, const bool dyn): type(tp), dynamic(dyn) {};
ezValue::~ezValue(){};
void ezValue::reference(void) { m_reference++; }
void ezValue::release(void) { m_reference--; }

ezInteger::ezInteger(int val, const bool dynamic): m_value(val), ezValue(EZ_VALUE_TYPE_INTEGER, dynamic) {};
const int ezInteger::value(void) { return m_value; }

ezString::ezString(const string val, const bool dynamic): m_value(val), ezValue(EZ_VALUE_TYPE_STRING, dynamic) {};
const string ezString::value(void){ return m_value; }

ezCarousel::ezCarousel(const bool dynamic): ezValue(EZ_VALUE_TYPE_CAROUSEL, dynamic) {};
ezNativeCarousel::ezNativeCarousel(const bool dynamic): ezValue(EZ_VALUE_TYPE_NATIVE_CAROUSEL, dynamic) {};
