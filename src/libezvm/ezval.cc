#include "ezvm/ezval.h"
#include <stdexcept>

ezValue::ezValue(const ezValueType tp, const bool dyn): m_reference(0), type(tp), dynamic(dyn) {}
ezValue::~ezValue(){}
size_t ezValue::reference(void) { m_reference++; return m_reference; }
size_t ezValue::release(void) { if(m_reference > 0) m_reference--; return m_reference; }
ezValue* ezValue::duplicate(void) { throw runtime_error("unable to duplicate");}
void ezValue::add(ezValue* v) { throw runtime_error("unable to add");}
void ezValue::sub(ezValue* v) { throw runtime_error("unable to substract");}
ezValue* ezValue::condition(void) {throw runtime_error("not subject to a condition");}
ezCondition::ezCondition(const bool zr, const bool neg, const bool ovf, const bool cry, const bool dynamic) : ezValue(EZ_VALUE_TYPE_CONDITION, dynamic), zero(zr), negative(neg), overflow(ovf), carry(cry) {}

ezNull::ezNull():ezValue(EZ_VALUE_TYPE_NULL, false) {}
ezNull* ezNull::instance() {
	static ezNull null;
	return &null;
}

ezBool::ezBool(bool val, const bool dynamic):ezValue(EZ_VALUE_TYPE_BOOL, dynamic), m_value(val) {}
const bool ezBool::value(void) { return m_value; }
ezValue* ezBool::condition(void) {return new ezCondition(!m_value, false, false, false);}

ezInteger::ezInteger(int val, const bool dynamic):ezValue(EZ_VALUE_TYPE_INTEGER, dynamic), m_value(val) {}
const int ezInteger::value(void) { return m_value; }
ezValue* ezInteger::duplicate(void) { return new ezInteger(m_value);}
void ezInteger::add(ezValue* v) {
	if(v->type == EZ_VALUE_TYPE_INTEGER) m_value += ((ezInteger*)v)->value();
	else throw runtime_error("unable to cast to integer");
}
void ezInteger::sub(ezValue* v) {
	if(v->type == EZ_VALUE_TYPE_INTEGER) m_value -= ((ezInteger*)v)->value();
	else throw runtime_error("unable to cast to integer");
}
ezValue* ezInteger::condition(void) {return new ezCondition((m_value) ? false : true, (m_value < 0) ? true : false, false, false);}


ezString::ezString(const string val, const bool dynamic):ezValue(EZ_VALUE_TYPE_STRING, dynamic), m_value(val) {}
const string ezString::value(void){ return m_value; }
ezValue* ezString::duplicate(void) { return new ezString(m_value);}
void ezString::add(ezValue* v) {
	if(v->type == EZ_VALUE_TYPE_STRING) m_value += ((ezString*)v)->value();
	else throw runtime_error("unable to cast to string");
}
ezValue* ezString::condition(void) {return new ezCondition((m_value.empty()) ? true : false, false, false, false);}

ezCarousel::ezCarousel(uint8_t args, uint8_t rets, size_t mems, const bool dynamic):
	ezValue(EZ_VALUE_TYPE_CAROUSEL, dynamic),
	nargs(args), nrets(rets), nmems(mems){}
ezCarousel::~ezCarousel(){}
ezNativeCarousel::ezNativeCarousel(const bool dynamic): ezValue(EZ_VALUE_TYPE_NATIVE_CAROUSEL, dynamic) {}
