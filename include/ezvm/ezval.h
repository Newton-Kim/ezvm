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
#include "ezobject.h"
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

enum ezBinaryOperation {
  EZ_BINARY_OPERATION_COMPARISON = 0,
  EZ_BINARY_OPERATION_SUBTRACTION,
  EZ_BINARY_OPERATION_MULTIPLICATION,
  EZ_BINARY_OPERATION_DIVISION,
  EZ_BINARY_OPERATION_MODULATION,
  EZ_BINARY_OPERATION_POW,
  EZ_BINARY_OPERATION_AND,
  EZ_BINARY_OPERATION_OR,
  EZ_BINARY_OPERATION_XOR,
  EZ_BINARY_OPERATION_LSL,
  EZ_BINARY_OPERATION_LSR,
  EZ_BINARY_OPERATION_MAX
};

class ezValue;

ezObject *fn_binary_generic_error(ezValue *vl, ezValue *vr, bool flip);
ezValue *fn_unary_generic_error(ezValue *v);

typedef ezObject *fnBinaryOperation(ezValue *, ezValue *, bool flip);

class ezValue : public ezObject {
protected:
  fnBinaryOperation ***m_fn_binary;

public:
  const ezValueType id;
  ezValue(const ezValueType id);
  virtual ~ezValue();

  virtual ezObject *condition(void);

  virtual bool to_bool(void);
  virtual int to_int(void);
  virtual double to_float(void);
  virtual complex<double> to_complex(void);
  virtual string to_string(void);

  virtual ezValue* add(ezValue* v, bool flip = false);
  virtual ezValue* subtract(ezValue* v, bool flip = false);
  virtual ezValue* multiply(ezValue* v, bool flip = false);
  virtual ezValue* divide(ezValue* v, bool flip = false);
  virtual ezValue* modulate(ezValue* v, bool flip = false);
  virtual ezValue* power(ezValue* v, bool flip = false);
  virtual ezValue* bitwise_and(ezValue* v, bool flip = false);
  virtual ezValue* bitwise_or(ezValue* v, bool flip = false);
  virtual ezValue* bitwise_xor(ezValue* v, bool flip = false);
  virtual ezValue* lsl(ezValue* v, bool flip = false);
  virtual ezValue* lsr(ezValue* v, bool flip = false);
  virtual ezObject* compare(ezValue* v, bool flip = false);
  virtual ezValue* negate(void);
  virtual ezValue* bitwise_not(void);

  ezObject *operate(ezBinaryOperation op, ezValue *v, bool flip = false);
};

class ezBool : public ezValue {
public:
  const bool value;
  ezBool(bool val);
  bool to_bool(void);
  ezObject* compare(ezValue* v, bool flip = false);
  ezValue* bitwise_not(void);
  
  ezObject *condition(void);
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
};

class ezString : public ezValue {
public:
  const string value;
  ezString(const string val);
  string to_string(void);
  ezValue* add(ezValue* v, bool flip = false);
  ezObject* compare(ezValue* v, bool flip = false);
  ezObject *condition(void);
};

