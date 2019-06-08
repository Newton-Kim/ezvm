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

enum ezUnaryOperation {
  EZ_UNARY_OPERATION_NEGATE = 0,
  EZ_UNARY_OPERATION_NOT,
  EZ_UNARY_OPERATION_MAX
};

enum ezBinaryOperation {
  EZ_BINARY_OPERATION_ADDITION = 0,
  EZ_BINARY_OPERATION_COMPARISON,
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
typedef ezValue *fnUnaryOperation(ezValue *);

class ezValue : public ezObject {
protected:
  fnBinaryOperation ***m_fn_binary;
  fnUnaryOperation **m_fn_unary;

public:
  const ezValueType id;
  ezValue(const ezValueType id);
  virtual ~ezValue();

  virtual ezObject *condition(void);

  ezObject *operate(ezBinaryOperation op, ezValue *v, bool flip = false);
  ezValue *operate(ezUnaryOperation op);
};

class ezBool : public ezValue {
public:
  const bool value;
  ezBool(bool val);
  ezObject *condition(void);
};

class ezInteger : public ezValue {
public:
  const int value;
  ezInteger(int val);
  ezObject *condition(void);
};

class ezFloat : public ezValue {
public:
  const double value;
  ezFloat(double val);
  ezObject *condition(void);
};

class ezComplex : public ezValue {
public:
  const complex<double> value;
  ezComplex(complex<double> val);
  ezObject *condition(void);
};

class ezString : public ezValue {
public:
  const string value;
  ezString(const string val);
  ezObject *condition(void);
};

