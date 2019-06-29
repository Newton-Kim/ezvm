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
#include "ezvm/ezfile.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

ezValue::ezValue(const unsigned int tp)
    : id(tp), ezObject(EZ_OBJECT_TYPE_VALUE) {
  m_size = sizeof(*this);
}
ezValue::~ezValue() {}

ezObject *ezValue::condition(void) {
  throw runtime_error("not subject to a condition");
}

ezValue *ezValue::add(ezValue *v, bool flip) {
  throw runtime_error("unable to add this type");
}

ezValue *ezValue::subtract(ezValue *v, bool flip) {
  throw runtime_error("unable to subtract this type");
}

ezValue *ezValue::multiply(ezValue *v, bool flip) {
  throw runtime_error("unable to multiply this type");
}

ezValue *ezValue::divide(ezValue *v, bool flip) {
  throw runtime_error("unable to divide this type");
}

ezValue *ezValue::modulate(ezValue *v, bool flip) {
  throw runtime_error("unable to modulate this type");
}

ezValue *ezValue::power(ezValue *v, bool flip) {
  throw runtime_error("unable to power this type");
}

ezValue *ezValue::bitwise_and(ezValue *v, bool flip) {
  throw runtime_error("unable to bitwise and this type");
}

ezValue *ezValue::bitwise_or(ezValue *v, bool flip) {
  throw runtime_error("unable to bitwise or this type");
}

ezValue *ezValue::bitwise_xor(ezValue *v, bool flip) {
  throw runtime_error("unable to bitwise xor this type");
}

ezValue *ezValue::lsl(ezValue *v, bool flip) {
  throw runtime_error("unable to left shift this type");
}

ezValue *ezValue::lsr(ezValue *v, bool flip) {
  throw runtime_error("unable to right shift this type");
}

ezObject *ezValue::compare(ezValue *v, bool flip) {
  throw runtime_error("unable to compare this type");
}

ezValue *ezValue::negate(void) {
  throw runtime_error("unable to negate this type");
}

ezValue *ezValue::bitwise_not(void) {
  throw runtime_error("unable to bitwise not this type");
}

bool ezValue::is_equal(ezValue *v) {
  throw runtime_error("unable to test equality of this type");
}

void ezValue::dump(ezFile &sink) { sink.print("(unknown)"); }
