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
#include <iostream>
#include <sstream>
#include <stdexcept>

static ezValue* fn_op_string_error(ezValue* vl, ezValue* vr) {
  throw runtime_error("compare string with given type is not supported");
}

static ezValue* fn_add_string_integer(ezValue* vl, ezValue* vr) {
  stringstream ss;
  ss << ((ezString*)vl)->value;
  ss << ((ezInteger*)vr)->value;
  return new ezString(ss.str());
}

static ezValue* fn_add_string_float(ezValue* vl, ezValue* vr) {
  stringstream ss;
  ss << ((ezString*)vl)->value;
  ss << ((ezFloat*)vr)->value;
  return new ezString(ss.str());
}

static ezValue* fn_add_string_complex(ezValue* vl, ezValue* vr) {
  stringstream ss;
  ss << ((ezString*)vl)->value;
  complex<double> vp = ((ezComplex*)vr)->value;
  ss << vp.real();
  if(vp.imag() >= 0) ss << "+";
  ss << vp.imag() << "j";
  return new ezString(ss.str());
}

static ezValue* fn_add_string_string(ezValue* vl, ezValue* vr) {
  return new ezString(((ezString*)vl)->value + ((ezString*)vr)->value);
}

static ezValue* fn_compare_string_string(ezValue* vl, ezValue* vr) {
  return new ezBool(((ezString*)vl)->value == ((ezString*)vr)->value);
}

static fnBinaryOperation* fn_add_string[] = {
  fn_op_string_error,
  fn_op_string_error,
  fn_op_string_error,
  fn_add_string_integer,
  fn_add_string_float,
  fn_add_string_complex,
  fn_add_string_string,
  fn_op_string_error,
  fn_op_string_error,
  fn_op_string_error
};

static fnBinaryOperation* fn_compare_string[] = {
  fn_op_string_error,
  fn_op_string_error,
  fn_op_string_error,
  fn_op_string_error,
  fn_op_string_error,
  fn_op_string_error,
  fn_compare_string_string,
  fn_op_string_error,
  fn_op_string_error,
  fn_op_string_error
};

ezString::ezString(const string val)
    : ezValue(EZ_VALUE_TYPE_STRING), value(val) {
  m_size = sizeof(*this) + val.size() + 1;
  m_fn_add = fn_add_string;
  m_fn_cmp = fn_compare_string;
}

ezValue *ezString::condition(void) {
  return new ezCondition(value.empty(), false, false, false);
}

