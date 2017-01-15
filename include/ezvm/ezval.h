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
#include "ezgc.h"
#include "eztable.h"
#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

using namespace std;

enum ezValueType {
  EZ_VALUE_TYPE_NULL = 0,
  EZ_VALUE_TYPE_CONDITION,
  EZ_VALUE_TYPE_BOOL,
  EZ_VALUE_TYPE_INTEGER,
  EZ_VALUE_TYPE_FLOAT,
  EZ_VALUE_TYPE_STRING,
  EZ_VALUE_TYPE_CAROUSEL,
  EZ_VALUE_TYPE_NATIVE_CAROUSEL,
  EZ_VALUE_TYPE_MAX
};

class ezValue : public ezGCObject{
 public:
  const ezValueType type;
  ezValue(const ezValueType tp);
  virtual ~ezValue();
  virtual bool to_bool(void);
  virtual int to_integer(void);
  virtual string to_string(void);
  virtual ezValue* condition(void);
  virtual double to_float(void);
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
  static ezNull* instance(void);
};

class ezBool : public ezValue {
 private:
  bool m_value;

 public:
  ezBool(bool val);
  bool to_bool(void);
  int to_integer(void);
  string to_string(void);
  ezValue* condition(void);
  virtual double to_float(void);
};

class ezInteger : public ezValue {
 private:
  int m_value;

 public:
  ezInteger(int val);
  bool to_bool(void);
  int to_integer(void);
  string to_string(void);
  ezValue* condition(void);
  virtual double to_float(void);
};

class ezFloat: public ezValue {
 private:
  double m_value;

 public:
  ezFloat(double val);
  bool to_bool(void);
  int to_integer(void);
  string to_string(void);
  ezValue* condition(void);
  virtual double to_float(void);
};

class ezString : public ezValue {
 private:
  string m_value;

 public:
  ezString(const string val);
  bool to_bool(void);
  int to_integer(void);
  string to_string(void);
  ezValue* condition(void);
  virtual double to_float(void);
};

typedef uint32_t ezInstruction;

class ezCarousel : public ezValue {
 private:
  ezTable<string, ezValue*>* m_scope;
  ezTable<string, ezValue*>* m_local;
 public:
  uint8_t nargs;
  uint8_t nrets;
  size_t nmems;
  ezCarousel(uint8_t args, uint8_t rets, size_t mems);
  ~ezCarousel();
  vector<ezInstruction> instruction;
  vector<ezInstruction> jmptbl;
  map<string, size_t> symtab;
};

class ezNativeCarousel : public ezValue {
 private:
 public:
  ezNativeCarousel();
  virtual void run(vector<ezValue*>& args, vector<ezValue*>& rets) = 0;
};
