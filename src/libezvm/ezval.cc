#include "ezvm/ezval.h"

ezValue::ezValue(ezValueType tp): type(tp) {};
ezValue::~ezValue(){};
void ezValue::reference(void) { m_reference++; }
void ezValue::release(void) { m_reference--; }

ezInteger::ezInteger(int val): m_value(val), ezValue(EZ_VALUE_TYPE_INTEGER) {};
const int ezInteger::value(void) { return m_value; }

ezString::ezString(const string val): m_value(val), ezValue(EZ_VALUE_TYPE_STRING) {};
const string ezString::value(void){ return m_value; }

ezNativeCarousel::ezNativeCarousel(): ezValue(EZ_VALUE_TYPE_NATIVE_CAROUSEL) {};
