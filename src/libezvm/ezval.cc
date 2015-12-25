/* Copyright (C) 2015 Newton Kim
*
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditiong:
*
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/
#include "ezvm/ezval.h"
#include <stdexcept>
#include <sstream>

ezValue::ezValue(const ezValueType tp, const bool dyn)
    : m_reference(0), type(tp), dynamic(dyn) {}
ezValue::~ezValue() {}
size_t ezValue::reference(void) {
  m_reference++;
  return m_reference;
}
size_t ezValue::release(void) {
  if (m_reference > 0) m_reference--;
  return m_reference;
}
bool ezValue::to_bool(void) { throw runtime_error("unable to cast to bool"); }
int ezValue::to_integer(void) {
  throw runtime_error("unable to cast to integer");
}
string ezValue::to_string(void) {
  throw runtime_error("unable to cast to string");
}
ezValue* ezValue::condition(void) {
  throw runtime_error("not subject to a condition");
}

ezCondition::ezCondition(const bool zr, const bool neg, const bool ovf,
                         const bool cry, const bool dynamic)
    : ezValue(EZ_VALUE_TYPE_CONDITION, dynamic),
      zero(zr),
      negative(neg),
      overflow(ovf),
      carry(cry) {}

ezNull::ezNull() : ezValue(EZ_VALUE_TYPE_NULL, false) {}
ezNull* ezNull::instance() {
  static ezNull null;
  return &null;
}

ezBool::ezBool(bool val, const bool dynamic)
    : ezValue(EZ_VALUE_TYPE_BOOL, dynamic), m_value(val) {}
bool ezBool::to_bool(void) { return m_value; }
int ezBool::to_integer(void) { return m_value ? 1 : 0; }
string ezBool::to_string(void) {
  string str;
  str = m_value ? "true" : "false";
}
ezValue* ezBool::condition(void) {
  return new ezCondition(!m_value, false, false, false);
}

ezInteger::ezInteger(int val, const bool dynamic)
    : ezValue(EZ_VALUE_TYPE_INTEGER, dynamic), m_value(val) {}
bool ezInteger::to_bool(void) { return m_value ? true : false; }
int ezInteger::to_integer(void) { return m_value; }
string ezInteger::to_string(void) {
  stringstream ss;
  ss << m_value;
  return ss.str();
}
ezValue* ezInteger::condition(void) {
  return new ezCondition((m_value) ? false : true, (m_value < 0) ? true : false,
                         false, false);
}

ezString::ezString(const string val, const bool dynamic)
    : ezValue(EZ_VALUE_TYPE_STRING, dynamic), m_value(val) {}
bool ezString::to_bool(void) {
  bool ret;
  if (m_value == "true")
    ret = true;
  else if (m_value == "false")
    ret = false;
  else
    throw runtime_error("unable to cast from string to bool");
}
int ezString::to_integer(void) { return m_value.size() ? 1 : 0; }
string ezString::to_string(void) { return m_value; }
ezValue* ezString::condition(void) {
  return new ezCondition((m_value.empty()) ? true : false, false, false, false);
}

ezCarousel::ezCarousel(uint8_t args, uint8_t rets, size_t mems,
                       const bool dynamic)
    : ezValue(EZ_VALUE_TYPE_CAROUSEL, dynamic),
      nargs(args),
      nrets(rets),
      nmems(mems) {}
ezCarousel::~ezCarousel() {}
ezNativeCarousel::ezNativeCarousel(const bool dynamic)
    : ezValue(EZ_VALUE_TYPE_NATIVE_CAROUSEL, dynamic) {}
