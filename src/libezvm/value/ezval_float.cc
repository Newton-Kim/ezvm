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

ezFloat::ezFloat(double val) : ezValue(EZ_VALUE_TYPE_FLOAT), m_value(val) {
  m_size = sizeof(*this);
}
bool ezFloat::to_bool(void) { return m_value ? true : false; }
int ezFloat::to_integer(void) { return (int)m_value; }
double ezFloat::to_float(void) { return m_value; }
complex<double> ezFloat::to_complex(void) {
  return complex<double>(m_value, 0);
}
string ezFloat::to_string(void) {
  stringstream ss;
  ss << m_value;
  return ss.str();
}

ezValue *ezFloat::condition(void) {
  return new ezCondition(m_value ? false : true, (m_value < 0) ? true : false,
                         false, false);
}

ezValue* ezFloat::add(ezValue* v) {
  return new ezFloat(m_value + v->to_float());
}

ezValue* ezFloat::subtract(ezValue* v) {
  return new ezFloat(m_value - v->to_float());
}

ezValue* ezFloat::multiply(ezValue* v) {
  return new ezFloat(m_value * v->to_float());
}

ezValue* ezFloat::divide(ezValue* v) {
  return new ezFloat(m_value / v->to_float());
}

ezValue* ezFloat::compare(ezValue* v) {
  double val = m_value - v->to_float();
  return new ezCondition((val ? false : true), ((val < 0) ? true : false),
                         false, false);
}

ezValue* ezFloat::negate(void) {
  return new ezFloat(-m_value);
}


