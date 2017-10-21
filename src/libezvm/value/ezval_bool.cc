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

static fnBinaryOperation *fn_add_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_sub_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_mul_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_div_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_mod_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_pow_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_cmp_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_compare_bool_bool,
    fn_compare_bool_integer, fn_compare_bool_float,   fn_compare_bool_complex,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_and_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_or_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_xor_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_lsl_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_lsr_bool[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation **fn_binary_bool[EZ_BINARY_OPERATION_MAX] = {
    fn_add_bool,  fn_cmp_bool,   fn_sub_bool, fn_mul_bool,
    fn_div_bool,  fn_mod_bool,   fn_pow_bool, fn_b_and_bool,
    fn_b_or_bool, fn_b_xor_bool, fn_lsl_bool, fn_lsr_bool};

static ezValue *fn_neg_bool(ezValue *v) {
  return new ezBool(!((ezBool *)v)->value);
}

static fnUnaryOperation *fn_unary_bool[EZ_UNARY_OPERATION_MAX] = {
    fn_neg_bool, fn_unary_generic_error};

ezBool::ezBool(bool val) : ezValue(EZ_VALUE_TYPE_BOOL), value(val) {
  m_size = sizeof(*this);
  m_fn_binary = fn_binary_bool;
  m_fn_unary = fn_unary_bool;
}

ezValue *ezBool::condition(void) {
  return new ezCondition(!value, false, false, false);
}

void ezBool::set_operation(ezBinaryOperation op, ezValueType type,
                           fnBinaryOperation *fn) {
  if (!fn)
    throw runtime_error("null function is not allowed");
  fn_binary_bool[op][type] = fn;
}

void ezBool::set_operation(ezUnaryOperation op, fnUnaryOperation *fn) {
  if (!fn)
    throw runtime_error("null function is not allowed");
  fn_unary_bool[op] = fn;
}
