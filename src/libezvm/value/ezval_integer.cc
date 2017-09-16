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

ezInteger::ezInteger(int val) : ezValue(EZ_VALUE_TYPE_INTEGER), m_value(val) {
  m_size = sizeof(*this);
}
bool ezInteger::to_bool(void) { return m_value ? true : false; }
int ezInteger::to_integer(void) { return m_value; }
double ezInteger::to_float(void) { return (double)m_value; }
complex<double> ezInteger::to_complex(void) {
  return complex<double>(m_value, 0);
}
string ezInteger::to_string(void) {
  stringstream ss;
  ss << m_value;
  return ss.str();
}

ezValue *ezInteger::condition(void) {
  return new ezCondition(m_value ? false : true, (m_value < 0) ? true : false,
                         false, false);
}

ezValue* ezInteger::add(ezValue* v) {
  return new ezInteger(m_value + v->to_integer());
}

ezValue* ezInteger::subtract(ezValue* v) {
  return new ezInteger(m_value - v->to_integer());
}

ezValue* ezInteger::multiply(ezValue* v) {
  return new ezInteger(m_value * v->to_integer());
}

ezValue* ezInteger::divide(ezValue* v) {
  return new ezInteger(m_value / v->to_integer());
}

ezValue* ezInteger::modulo(ezValue* v) {
  return new ezInteger(m_value % v->to_integer());
}

ezValue* ezInteger::bitwise_and(ezValue* v) {
  return new ezInteger(m_value & v->to_integer());
}

ezValue* ezInteger::bitwise_or(ezValue* v) {
  return new ezInteger(m_value | v->to_integer());
}

ezValue* ezInteger::bitwise_xor(ezValue* v) {
  return new ezInteger(m_value ^ v->to_integer());
}

ezValue* ezInteger::bitwise_not(void) {
  return new ezInteger(~m_value);
}

ezValue* ezInteger::compare(ezValue* v) {
  int val = m_value - v->to_integer();
  return new ezCondition((val ? false : true), ((val < 0) ? true : false), false,
                         false);
}

ezValue* ezInteger::negate(void) {
  return new ezInteger(-m_value);
}


