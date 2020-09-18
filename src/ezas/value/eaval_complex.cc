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

ezComplex::ezComplex(complex<double> val)
    : eaValue(EZ_VALUE_TYPE_COMPLEX), value(val) {
  m_size = sizeof(*this);
}

complex<double> ezComplex::to_complex(void) { return value; }

string ezComplex::to_string(void) {
  stringstream ss;
  ss << value.real();
  if (value.imag() >= 0)
    ss << "+";
  ss << value.imag() << "j";
  return ss.str();
}

void ezComplex::dump(ezFile &sink) {
  double cr = value.real();
  double ci = value.imag();
  sink.print("%f %s %fj\n", cr, (ci > 0 ? "+" : "-"), ci);
}

ezValue *ezComplex::negate(void) { return new ezComplex(-value); }
