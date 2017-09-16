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
#include <iostream>
#include <sstream>
#include <stdexcept>

ezString::ezString(const string val)
    : ezValue(EZ_VALUE_TYPE_STRING), m_value(val) {
  m_size = sizeof(*this) + val.size() + 1;
}
bool ezString::to_bool(void) {
  bool ret;
  if (m_value == "true")
    ret = true;
  else if (m_value == "false")
    ret = false;
  else
    throw runtime_error("unable to cast from string to bool");
  return ret;
}
int ezString::to_integer(void) { return m_value.size() ? 1 : 0; }
double ezString::to_float(void) { return m_value.size() ? 1. : 0; }
string ezString::to_string(void) { return m_value; }
ezValue *ezString::condition(void) {
  return new ezCondition(m_value.empty(), false, false, false);
}

ezValue* ezString::add(ezValue* v) {
  return new ezString(m_value + v->to_string());
}

ezValue* ezString::compare(ezValue* v) {
  string vstr = v->to_string();
  bool eq = (m_value == vstr);
  int sz = m_value.size() - vstr.size();
  return new ezCondition(eq, ((sz < 0) ? true : false), false, false);
}

