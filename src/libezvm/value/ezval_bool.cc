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


ezBool::ezBool(bool val) : ezValue(EZ_VALUE_TYPE_BOOL), m_value(val) {
  m_size = sizeof(*this);
}
bool ezBool::to_bool(void) { return m_value; }
int ezBool::to_integer(void) { return m_value ? 1 : 0; }
double ezBool::to_float(void) { return m_value ? 1. : 0; }
complex<double> ezBool::to_complex(void) {
  return complex<double>((m_value ? 1. : 0), 0);
}
string ezBool::to_string(void) {
  string str;
  str = m_value ? "true" : "false";
  return str;
}
ezValue *ezBool::condition(void) {
  return new ezCondition(!m_value, false, false, false);
}


