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
#include "ezaddr.h"
#include "ezgc.h"
#include "ezinstruction.h"
#include "eztable.h"
#include <complex>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum ezValueType {
  EZ_VALUE_TYPE_NULL = 0,
  EZ_VALUE_TYPE_CONDITION,
  EZ_VALUE_TYPE_BOOL,
  EZ_VALUE_TYPE_INTEGER,
  EZ_VALUE_TYPE_FLOAT,
  EZ_VALUE_TYPE_COMPLEX,
  EZ_VALUE_TYPE_STRING,
  EZ_VALUE_TYPE_FUNCTION,
  EZ_VALUE_TYPE_USER_DEFINED_FUNCTION,
  EZ_VALUE_TYPE_USER_DEFINED_VALUE,
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

ezValue *fn_binary_generic_error(ezValue *vl, ezValue *vr);
ezValue *fn_unary_generic_error(ezValue *v);

typedef ezValue *fnBinaryOperation(ezValue *, ezValue *);
typedef ezValue *fnUnaryOperation(ezValue *);

class ezValue : public ezGCObject {
protected:
  fnBinaryOperation ***m_fn_binary;
  fnUnaryOperation **m_fn_unary;

public:
  const ezValueType type;
  ezValue(const ezValueType tp);
  virtual ~ezValue();

  virtual ezValue *condition(void);

  ezValue *operate(ezBinaryOperation op, ezValue *v);
  ezValue *operate(ezUnaryOperation op);
};

class ezCondition : public ezValue {
public:
  const bool zero;
  const bool negative;
  const bool overflow;
  const bool carry;
  ezCondition(const bool zr, const bool neg, const bool ovf, const bool cry);
};

class ezNull : public ezValue {
public:
  ezNull();
  static ezNull *instance(void);
};

class ezBool : public ezValue {
public:
  const bool value;
  ezBool(bool val);
  ezValue *condition(void);

  static void set_operation(ezBinaryOperation op, ezValueType type, fnBinaryOperation *fn);
  static void set_operation(ezUnaryOperation op, fnUnaryOperation *fn);
};

class ezInteger : public ezValue {
public:
  const int value;
  ezInteger(int val);
  ezValue *condition(void);

  static void set_operation(ezBinaryOperation op, ezValueType type, fnBinaryOperation *fn);
  static void set_operation(ezUnaryOperation op, fnUnaryOperation *fn);
};

class ezFloat : public ezValue {
public:
  const double value;
  ezFloat(double val);
  ezValue *condition(void);

  static void set_operation(ezBinaryOperation op, ezValueType type, fnBinaryOperation *fn);
  static void set_operation(ezUnaryOperation op, fnUnaryOperation *fn);
};

class ezComplex : public ezValue {
public:
  const complex<double> value;
  ezComplex(complex<double> val);
  ezValue *condition(void);

  static void set_operation(ezBinaryOperation op, ezValueType type, fnBinaryOperation *fn);
  static void set_operation(ezUnaryOperation op, fnUnaryOperation *fn);
};

class ezString : public ezValue {
public:
  const string value;
  ezString(const string val);
  ezValue *condition(void);

  static void set_operation(ezBinaryOperation op, ezValueType type, fnBinaryOperation *fn);
  static void set_operation(ezUnaryOperation op, fnUnaryOperation *fn);
};

class ezFunction : public ezValue, ezGCClient {
private:
  ezTable<string, ezValue *> *m_scope;
  ezTable<string, ezValue *> *m_local;

public:
  uint8_t nargs;
  size_t nmems;
  ezFunction(ezTable<string, ezValue *> *local,
             ezTable<string, ezValue *> *scope);
  ~ezFunction();
  vector<ezInstruction *> instruction;
  ezTable<string, size_t> jmptbl;
  void on_mark(void);
  vector<ezValue *> *local_memory(void);
  vector<ezValue *> *scope_memory(void);
  bool is_local_scoped(void) { return (m_local) ? true : false; }
};

class ezUserDefinedFunction : public ezValue {
public:
  ezUserDefinedFunction();
  virtual ~ezUserDefinedFunction() {}

  virtual void run(vector<ezValue *> &args, vector<ezValue *> &rets) = 0;
};

class ezUserDefinedValue : public ezValue {
public:
  ezUserDefinedValue();
  virtual ~ezUserDefinedValue();
};
