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
#pragma once
#include "ezfile.h"
#include "ezobject.h"
#include <complex>

using namespace std;

class ezValue : public ezObject {
public:
  const unsigned int id;
  ezValue(const unsigned int id);
  virtual ~ezValue();

  virtual ezObject *condition(void);
  virtual ezValue *add(ezValue *v, bool flip = false);
  virtual ezValue *subtract(ezValue *v, bool flip = false);
  virtual ezValue *multiply(ezValue *v, bool flip = false);
  virtual ezValue *divide(ezValue *v, bool flip = false);
  virtual ezValue *modulate(ezValue *v, bool flip = false);
  virtual ezValue *power(ezValue *v, bool flip = false);
  virtual ezValue *bitwise_and(ezValue *v, bool flip = false);
  virtual ezValue *bitwise_or(ezValue *v, bool flip = false);
  virtual ezValue *bitwise_xor(ezValue *v, bool flip = false);
  virtual ezValue *lsl(ezValue *v, bool flip = false);
  virtual ezValue *lsr(ezValue *v, bool flip = false);
  virtual ezObject *compare(ezValue *v, bool flip = false);
  virtual ezValue *negate(void);
  virtual ezValue *bitwise_not(void);
  virtual bool is_equal(ezValue *v);
  virtual void dump(ezFile &sink);
};
