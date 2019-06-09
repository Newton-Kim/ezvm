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
#include "ezvm/ezfunc.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

ezFloat::ezFloat(double val) : ezValue(EZ_VALUE_TYPE_FLOAT), value(val) {
  m_size = sizeof(*this);
}

int ezFloat::to_int(void) {
  return value;
}

double ezFloat::to_float(void) {
  return value;
}

complex<double> ezFloat::to_complex(void) {
  return value;
}

string ezFloat::to_string(void) {
  stringstream ss;
  ss << value;
  return ss.str();
}

ezValue* ezFloat::add(ezValue* v, bool flip) {
  return new ezFloat(value + v->to_float());
}

ezValue* ezFloat::subtract(ezValue* v, bool flip) {
  double ret = (flip) ? v->to_float() - value : value - v->to_float();
  return new ezFloat(ret);
}

ezValue* ezFloat::multiply(ezValue* v, bool flip) {
  return new ezFloat(value * v->to_float());
}

ezValue* ezFloat::divide(ezValue* v, bool flip) {
  double ret = (flip) ? v->to_float() / value : value / v->to_float();
  return new ezFloat(ret);
}

ezValue* ezFloat::power(ezValue* v, bool flip) {
  double ret = (flip) ? pow(v->to_float(), value) : pow(value, v->to_float());
  return new ezFloat(ret);
}

ezObject* ezFloat::compare(ezValue* v, bool flip) {
  double diff = value - v->to_float();
  return new ezCondition(v->id == EZ_VALUE_TYPE_FLOAT && 0 == diff, (diff < 0) ? true : false, false, false);
}

ezValue* ezFloat::negate(void) {
  return new ezFloat(-value);
}

ezObject *ezFloat::condition(void) {
  return new ezCondition(value ? false : true, (value < 0) ? true : false,
                         false, false);
}

