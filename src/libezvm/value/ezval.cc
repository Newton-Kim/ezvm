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

ezValue::ezValue(const ezValueType tp) : type(tp) { m_size = sizeof(*this); }
ezValue::~ezValue() {}

bool ezValue::to_bool(void) { throw runtime_error("unable to cast to bool"); }
int ezValue::to_integer(void) {
  throw runtime_error("unable to cast to integer");
}
double ezValue::to_float(void) {
  throw runtime_error("unable to cast to float");
}
complex<double> ezValue::to_complex(void) {
  throw runtime_error("unable to cast to complex");
}
string ezValue::to_string(void) {
  throw runtime_error("unable to cast to string");
}
ezValue *ezValue::condition(void) {
  throw runtime_error("not subject to a condition");
}

ezValue *ezValue::add(ezValue *v) {
  throw runtime_error("addition is not supported");
}

ezValue *ezValue::subtract(ezValue *v) {
  throw runtime_error("subtraction is not supported");
}

ezValue *ezValue::multiply(ezValue *v) {
  throw runtime_error("multiplication is not supported");
}

ezValue *ezValue::divide(ezValue *v) {
  throw runtime_error("division is not supported");
}

ezValue *ezValue::modulo(ezValue *v) {
  throw runtime_error("modulo is not supported");
}

ezValue *ezValue::bitwise_and(ezValue *v) {
  throw runtime_error("bitwise and is not supported");
}

ezValue *ezValue::bitwise_or(ezValue *v) {
  throw runtime_error("bitwise or is not supported");
}

ezValue *ezValue::bitwise_xor(ezValue *v) {
  throw runtime_error("bitwise xor is not supported");
}

ezValue *ezValue::bitwise_not(void) {
  throw runtime_error("bitwise not is not supported");
}

ezValue *ezValue::compare(ezValue *v) {
  throw runtime_error("comparison is not supported");
}

ezValue *ezValue::negate(void) {
  throw runtime_error("negation is not supported");
}

ezValue *ezValue::powv(ezValue *v) {
  throw runtime_error("power is not supported");
}
