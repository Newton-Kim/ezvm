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
#include "ezvm/ezval.h"
#include <complex>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum ezValueType {
  EZ_VALUE_TYPE_UNDEFINED = 0,
  EZ_VALUE_TYPE_BOOL,
  EZ_VALUE_TYPE_INTEGER,
  EZ_VALUE_TYPE_FLOAT,
  EZ_VALUE_TYPE_COMPLEX,
  EZ_VALUE_TYPE_STRING,
  EZ_VALUE_TYPE_MAX
};

class ezBool : public ezValue {
public:
  const bool value;
  ezBool(bool val);
  bool to_bool(void);
  ezObject* compare(ezValue* v, bool flip = false);
  ezValue* bitwise_not(void);
  ezObject *condition(void);
  bool is_equal(ezValue* v);
  void dump(ezFile &sink);
};

class ezInteger : public ezValue {
public:
  const int value;
  ezInteger(int val);
  int to_int(void);
  double to_float(void);
  complex<double> to_complex(void);
  string to_string(void);
  ezValue* add(ezValue* v, bool flip = false);
  ezValue* subtract(ezValue* v, bool flip = false);
  ezValue* multiply(ezValue* v, bool flip = false);
  ezValue* divide(ezValue* v, bool flip = false);
  ezValue* modulate(ezValue* v, bool flip = false);
  ezValue* power(ezValue* v, bool flip = false);
  ezValue* bitwise_and(ezValue* v, bool flip = false);
  ezValue* bitwise_or(ezValue* v, bool flip = false);
  ezValue* bitwise_xor(ezValue* v, bool flip = false);
  ezValue* lsl(ezValue* v, bool flip = false);
  ezValue* lsr(ezValue* v, bool flip = false);
  ezObject* compare(ezValue* v, bool flip = false);
  ezValue* negate(void);
  ezValue* bitwise_not(void);
  ezObject *condition(void);
  bool is_equal(ezValue* v);
  void dump(ezFile &sink);
};

class ezFloat : public ezValue {
public:
  const double value;
  ezFloat(double val);
  int to_int(void);
  double to_float(void);
  complex<double> to_complex(void);
  string to_string(void);
  ezValue* add(ezValue* v, bool flip = false);
  ezValue* subtract(ezValue* v, bool flip = false);
  ezValue* multiply(ezValue* v, bool flip = false);
  ezValue* divide(ezValue* v, bool flip = false);
  ezValue* power(ezValue* v, bool flip = false);
  ezObject* compare(ezValue* v, bool flip = false);
  ezValue* negate(void);
  ezObject *condition(void);
  bool is_equal(ezValue* v);
  void dump(ezFile &sink);
};

class ezComplex : public ezValue {
public:
  const complex<double> value;
  ezComplex(complex<double> val);
  complex<double> to_complex(void);
  string to_string(void);
  ezValue* add(ezValue* v, bool flip = false);
  ezValue* subtract(ezValue* v, bool flip = false);
  ezValue* multiply(ezValue* v, bool flip = false);
  ezValue* divide(ezValue* v, bool flip = false);
  ezValue* power(ezValue* v, bool flip = false);
  ezObject* compare(ezValue* v, bool flip = false);
  ezValue* negate(void);
  ezObject *condition(void);
  bool is_equal(ezValue* v);
  void dump(ezFile &sink);
};

class ezString : public ezValue {
public:
  const string value;
  ezString(const string val);
  string to_string(void);
  ezValue* add(ezValue* v, bool flip = false);
  ezObject* compare(ezValue* v, bool flip = false);
  ezObject *condition(void);
  bool is_equal(ezValue* v);
  void dump(ezFile &sink);
};

