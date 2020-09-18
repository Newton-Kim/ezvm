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

ezInteger::ezInteger(int val) : eaValue(EZ_VALUE_TYPE_INTEGER), value(val) {
  m_size = sizeof(*this);
}

int ezInteger::to_int(void) { return value; }

double ezInteger::to_float(void) { return value; }

complex<double> ezInteger::to_complex(void) { return value; }

string ezInteger::to_string(void) {
  stringstream ss;
  ss << value;
  return ss.str();
}

ezValue *ezInteger::bitwise_not(void) { return new ezInteger(~value); }

ezValue *ezInteger::negate(void) { return new ezInteger(-value); }

void ezInteger::dump(ezFile &sink) { sink.print("%d\n", value); }

ezValue *ezInteger::power(ezValue *arg) {
  return new ezInteger(pow(value, ((eaValue *)arg)->to_int()));
}

ezValue *ezInteger::lsl(ezValue *arg) {
  return new ezInteger(value << ((eaValue *)arg)->to_int());
}

ezValue *ezInteger::lsr(ezValue *arg) {
  return new ezInteger(value >> ((eaValue *)arg)->to_int());
}

ezObject *ezInteger::compare(ezValue *arg) {
  int diff = value - ((eaValue *)arg)->to_int();
  return new ezCondition(arg->id == EZ_VALUE_TYPE_INTEGER && 0 == diff,
                         (diff < 0) ? true : false, false, false);
}

ezObject *ezInteger::condition(void) {
  return new ezCondition(0 == value, (value < 0) ? true : false, false, false);
}

bool ezInteger::is_equal(ezValue *arg) {
  if (EZ_VALUE_TYPE_INTEGER != arg->id)
    return false;
  if (value != ((eaValue *)arg)->to_int())
    return false;
  return true;
}
