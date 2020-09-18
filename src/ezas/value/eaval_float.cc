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
#include "eaval.h"
#include "ezvm/ezfunc.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

ezFloat::ezFloat(double val) : eaValue(EZ_VALUE_TYPE_FLOAT), value(val) {
  m_size = sizeof(*this);
}

int ezFloat::to_int(void) { return value; }

double ezFloat::to_float(void) { return value; }

complex<double> ezFloat::to_complex(void) { return value; }

string ezFloat::to_string(void) {
  stringstream ss;
  ss << value;
  return ss.str();
}

ezValue *ezFloat::negate(void) { return new ezFloat(-value); }

void ezFloat::dump(ezFile &sink) {}

ezValue *ezFloat::power(ezValue *arg) {
  return new ezFloat(pow(value, ((eaValue *)arg)->to_float()));
}

ezObject *ezFloat::compare(ezValue *arg) {
  double diff = value - ((eaValue *)arg)->to_float();
  return new ezCondition(arg->id == EZ_VALUE_TYPE_FLOAT && 0 == diff,
                         (diff < 0) ? true : false, false, false);
}

ezObject *ezFloat::condition() {
  return new ezCondition(0 == value, (value < 0) ? true : false, false, false);
}

bool ezFloat::is_equal(ezValue *arg) {
  if (EZ_VALUE_TYPE_FLOAT != arg->id)
    return false;
  if (value != ((eaValue *)arg)->to_float())
    return false;
  return true;
}
