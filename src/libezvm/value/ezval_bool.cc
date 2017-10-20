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

static ezValue *fn_compare_bool_error(ezValue *vl, ezValue *vr) {
  throw runtime_error("compare bool with given type is not supported");
}

static ezValue *fn_compare_bool_bool(ezValue *vl, ezValue *vr) {
  bool bvl = ((ezBool *)vl)->value;
  bool bvr = ((ezBool *)vr)->value;
  return new ezBool(!(bvl ^ bvr));
}

static ezValue *fn_compare_bool_integer(ezValue *vl, ezValue *vr) {
  bool bvl = ((ezBool *)vl)->value;
  bool bvr = ((ezInteger *)vr)->value ? true : false;
  return new ezBool(bvl == bvr);
}

static ezValue *fn_compare_bool_float(ezValue *vl, ezValue *vr) {
  bool bvl = ((ezBool *)vl)->value;
  bool bvr = ((ezFloat *)vr)->value ? true : false;
  return new ezBool(bvl == bvr);
}

static ezValue *fn_compare_bool_complex(ezValue *vl, ezValue *vr) {
  bool bvl = ((ezBool *)vl)->value;
  bool bvr = abs(((ezFloat *)vr)->value) ? true : false;
  return new ezBool(bvl == bvr);
}

static fnBinaryOperation *fn_compare_bool[] = {
    fn_compare_bool_error,   fn_compare_bool_error, fn_compare_bool_bool,
    fn_compare_bool_integer, fn_compare_bool_float, fn_compare_bool_complex,
    fn_compare_bool_error,   fn_compare_bool_error, fn_compare_bool_error,
    fn_compare_bool_error};

static ezValue *fn_negate_bool(ezValue *v) {
  return new ezBool(!((ezBool *)v)->value);
}

ezBool::ezBool(bool val) : ezValue(EZ_VALUE_TYPE_BOOL), value(val) {
  m_size = sizeof(*this);
  m_fn_binary[EZ_BINARY_OPERATION_COMPARISON] = fn_compare_bool;
  m_fn_unary[EZ_UNARY_OPERATION_NEGATE] = fn_negate_bool;
}

ezValue *ezBool::condition(void) {
  return new ezCondition(!value, false, false, false);
}

void ezBool::set_operation(ezBinaryOperation op, fnBinaryOperation *fn) {
  // TODO:how to implement
}

void ezBool::set_operation(ezUnaryOperation op, fnUnaryOperation *fn) {
  // TODO:how to implement
}
