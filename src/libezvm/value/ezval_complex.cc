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

ezComplex::ezComplex(complex<double> val)
    : ezValue(EZ_VALUE_TYPE_COMPLEX), value(val) {
  m_size = sizeof(*this);
}

complex<double> ezComplex::to_complex(void) {
  return value;
}

string ezComplex::to_string(void) {
  stringstream ss;
  ss << value.real();
  if (value.imag() >= 0)
    ss << "+";
  ss << value.imag() << "j";
  return ss.str();
}

ezValue* ezComplex::add(ezValue* v, bool flip) {
  return new ezComplex(value + v->to_complex());
}

ezValue* ezComplex::subtract(ezValue* v, bool flip) {
  complex<double> ret = (flip) ? v->to_complex() - value : value - v->to_complex();
  return new ezComplex(ret);
}

ezValue* ezComplex::multiply(ezValue* v, bool flip) {
  return new ezComplex(value * v->to_complex());
}

ezValue* ezComplex::divide(ezValue* v, bool flip) {
  complex<double> ret = (flip) ? v->to_complex() / value : value / v->to_complex();
  return new ezComplex(ret);
}

ezValue* ezComplex::power(ezValue* v, bool flip) {
  complex<double> ret = (flip) ? pow(v->to_complex(), value) : pow(value, v->to_complex());
  return new ezComplex(ret);
}

ezObject* ezComplex::compare(ezValue* v, bool flip) {
  return new ezCondition(v->id == EZ_VALUE_TYPE_COMPLEX && value == v->to_complex(), false, false, false);
}

ezValue* ezComplex::negate(void) {
  return new ezComplex(-value);
}

ezObject *ezComplex::condition(void) {
  return new ezCondition(abs(value) ? false : true,
                         (abs(value) < 0) ? true : false, false, false);
}

