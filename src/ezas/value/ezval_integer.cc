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

ezInteger::ezInteger(int val) : ezValue(EZ_VALUE_TYPE_INTEGER), value(val) {
  m_size = sizeof(*this);
}

int ezInteger::to_int(void) {
  return value;
}

double ezInteger::to_float(void) {
  return value;
}

complex<double> ezInteger::to_complex(void) {
  return value;
}

string ezInteger::to_string(void) {
  stringstream ss;
  ss << value;
  return ss.str();
}

ezValue* ezInteger::add(ezValue* v, bool flip) {
  return new ezInteger(value + v->to_int());
  throw runtime_error("unable to add this type");
}

ezValue* ezInteger::subtract(ezValue* v, bool flip) {
  int ret = (flip) ? v->to_int() - value : value - v->to_int();
  return new ezInteger(ret);
}

ezValue* ezInteger::multiply(ezValue* v, bool flip) {
  return new ezInteger(value * v->to_int());
}

ezValue* ezInteger::divide(ezValue* v, bool flip) {
  int ret = (flip) ? v->to_int() / value : value / v->to_int();
  return new ezInteger(ret);
}

ezValue* ezInteger::modulate(ezValue* v, bool flip) {
  int ret = (flip) ? v->to_int() % value : value % v->to_int();
  return new ezInteger(ret);
}

ezValue* ezInteger::power(ezValue* v, bool flip) {
  int ret = (flip) ? pow(v->to_int(), value) : pow(value, v->to_int());
  return new ezInteger(ret);
}

ezValue* ezInteger::bitwise_and(ezValue* v, bool flip) {
  return new ezInteger(value & v->to_int());
}

ezValue* ezInteger::bitwise_or(ezValue* v, bool flip) {
  return new ezInteger(value | v->to_int());
}

ezValue* ezInteger::bitwise_xor(ezValue* v, bool flip) {
  return new ezInteger(value ^ v->to_int());
}

ezValue* ezInteger::lsl(ezValue* v, bool flip) {
  int ret = (flip) ? v->to_int() << value : value << v->to_int();
  return new ezInteger(ret);
}

ezValue* ezInteger::lsr(ezValue* v, bool flip) {
  int ret = (flip) ? v->to_int() >> value : value >> v->to_int();
  return new ezInteger(ret);
}

ezObject* ezInteger::compare(ezValue* v, bool flip) {
  int diff = value - v->to_int();
  return new ezCondition(v->id == EZ_VALUE_TYPE_INTEGER && 0 == diff, (diff < 0) ? true : false, false, false);
}

ezValue* ezInteger::negate(void) {
  return new ezInteger(-value);
}

ezValue* ezInteger::bitwise_not(void) {
  return new ezInteger(~value);
}

ezObject *ezInteger::condition(void) {
  return new ezCondition(value ? false : true, (value < 0) ? true : false,
                         false, false);
}

bool ezInteger::is_equal(ezValue*v) {
  if(EZ_VALUE_TYPE_INTEGER != v->id) return false;
  if(value != ((ezInteger*)v)->value) return false;
  return true;
}

void ezInteger::dump(ezFile &sink) {
    sink.print("%d\n", value);
}
