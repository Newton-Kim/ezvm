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

enum eaValueType {
  EZ_VALUE_TYPE_BOOL = 0,
  EZ_VALUE_TYPE_INTEGER,
  EZ_VALUE_TYPE_FLOAT,
  EZ_VALUE_TYPE_COMPLEX,
  EZ_VALUE_TYPE_STRING,
  EZ_VALUE_TYPE_MAX
};

class eaValue : public ezValue {
public:
  eaValue(const unsigned int id);
  virtual bool to_bool(void);
  virtual int to_int(void);
  virtual double to_float(void);
  virtual complex<double> to_complex(void);
  virtual string to_string(void);
};

class ezBool : public eaValue {
public:
  const bool value;
  ezBool(bool val);
  bool to_bool(void);
  void dump(ezFile &sink);
  ezObject *compare(ezValue *arg);
  ezObject *condition(void);
  ezValue *bitwise_not(void);
  bool is_equal(ezValue *arg);
};

class ezInteger : public eaValue {
public:
  const int value;
  ezInteger(int val);
  int to_int(void);
  double to_float(void);
  complex<double> to_complex(void);
  string to_string(void);
  void dump(ezFile &sink);
  ezValue *negate(void);
  ezValue *bitwise_not(void);
  virtual ezValue *power(ezValue *arg);
  virtual ezValue *lsl(ezValue *arg);
  virtual ezValue *lsr(ezValue *arg);
  virtual ezValue *bitwise_and(ezValue *arg);
  virtual ezValue *bitwise_or(ezValue *arg);
  virtual ezValue *bitwise_xor(ezValue *arg);
  virtual ezObject *compare(ezValue *arg);
  virtual ezObject *condition(void);
  virtual bool is_equal(ezValue *arg);
};

class ezFloat : public eaValue {
public:
  const double value;
  ezFloat(double val);
  int to_int(void);
  double to_float(void);
  complex<double> to_complex(void);
  string to_string(void);
  void dump(ezFile &sink);
  ezValue *negate(void);
  ezValue *power(ezValue *arg);
  ezObject *compare(ezValue *arg);
  ezObject *condition(void);
  bool is_equal(ezValue *arg);
};

class ezComplex : public eaValue {
public:
  const complex<double> value;
  ezComplex(complex<double> val);
  complex<double> to_complex(void);
  string to_string(void);
  void dump(ezFile &sink);
  ezValue *negate(void);
  ezValue *power(ezValue *arg);
  ezObject *compare(ezValue *arg);
  ezObject *condition(void);
  bool is_equal(ezValue *arg);
};

class ezString : public eaValue {
public:
  const string value;
  ezString(const string val);
  string to_string(void);
  void dump(ezFile &sink);
  virtual ezObject *compare(ezValue *arg);
  virtual ezObject *condition(void);
  virtual bool is_equal(ezValue *arg);
};
