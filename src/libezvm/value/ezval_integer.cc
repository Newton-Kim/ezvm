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

static ezValue *fn_add_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  return new ezInteger(((ezInteger *)vl)->value + ((ezInteger *)vr)->value);
}

static ezValue *fn_sub_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  int ret = (flip) ? ((ezInteger *)vr)->value - ((ezInteger *)vl)->value : ((ezInteger *)vl)->value - ((ezInteger *)vr)->value;
  return new ezInteger(ret);
}

static ezValue *fn_mul_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  return new ezInteger(((ezInteger *)vl)->value * ((ezInteger *)vr)->value);
}

static ezValue *fn_div_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  int ret = (flip) ? ((ezInteger *)vr)->value / ((ezInteger *)vl)->value : ((ezInteger *)vl)->value / ((ezInteger *)vr)->value;
  return new ezInteger(ret);
}

static ezValue *fn_mod_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  int ret = (flip) ? ((ezInteger *)vr)->value % ((ezInteger *)vl)->value : ((ezInteger *)vl)->value % ((ezInteger *)vr)->value;
  return new ezInteger(ret);
}

static ezValue *fn_b_and_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  return new ezInteger(((ezInteger *)vl)->value & ((ezInteger *)vr)->value);
}

static ezValue *fn_b_or_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  return new ezInteger(((ezInteger *)vl)->value | ((ezInteger *)vr)->value);
}

static ezValue *fn_b_xor_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  return new ezInteger(((ezInteger *)vl)->value ^ ((ezInteger *)vr)->value);
}

static ezValue *fn_lsl_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  int ret = (flip) ? ((ezInteger *)vr)->value << ((ezInteger *)vl)->value: ((ezInteger *)vl)->value << ((ezInteger *)vr)->value;
  return new ezInteger(ret);
}

static ezValue *fn_lsr_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  int ret = (flip) ? ((ezInteger *)vr)->value >> ((ezInteger *)vl)->value: ((ezInteger *)vl)->value >> ((ezInteger *)vr)->value;
  return new ezInteger(ret);
}

static ezValue *fn_pow_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  int ret = (flip) ? pow(((ezInteger *)vr)->value, ((ezInteger *)vl)->value): pow(((ezInteger *)vl)->value, ((ezInteger *)vr)->value);
  return new ezComplex(ret);
}

static ezValue *fn_cmp_integer_integer(ezValue *vl, ezValue *vr, bool flip) {
  int ret = (flip) ? ((ezInteger *)vr)->value - ((ezInteger *)vl)->value : ((ezInteger *)vl)->value - ((ezInteger *)vr)->value;
  return new ezCondition(ret == 0, ret < 0, false, false);
}

static fnBinaryOperation *fn_add_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_add_integer_integer};

static fnBinaryOperation *fn_sub_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_sub_integer_integer};

static fnBinaryOperation *fn_mul_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_mul_integer_integer};

static fnBinaryOperation *fn_div_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_div_integer_integer};

static fnBinaryOperation *fn_mod_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_mod_integer_integer};

static fnBinaryOperation *fn_pow_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_pow_integer_integer};

static fnBinaryOperation *fn_cmp_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_cmp_integer_integer};

static fnBinaryOperation *fn_b_and_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error,  fn_binary_generic_error, fn_b_and_integer_integer};

static fnBinaryOperation *fn_b_or_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_b_or_integer_integer};

static fnBinaryOperation *fn_b_xor_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error,  fn_binary_generic_error, fn_b_xor_integer_integer};

static fnBinaryOperation *fn_lsl_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_lsl_integer_integer};

static fnBinaryOperation *fn_lsr_integer[EZ_VALUE_TYPE_INTEGER + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_lsr_integer_integer};

static fnBinaryOperation **fn_binary_integer[EZ_BINARY_OPERATION_MAX + 1] = {
    fn_add_integer,  fn_cmp_integer,   fn_sub_integer, fn_mul_integer,
    fn_div_integer,  fn_mod_integer,   fn_pow_integer, fn_b_and_integer,
    fn_b_or_integer, fn_b_xor_integer, fn_lsl_integer, fn_lsr_integer};

static ezValue *fn_b_not_integer(ezValue *v) {
  return new ezFloat(~((ezInteger *)v)->value);
}

static ezValue *fn_neg_integer(ezValue *v) {
  return new ezInteger(-((ezInteger *)v)->value);
}

static fnUnaryOperation *fn_unary_integer[EZ_UNARY_OPERATION_MAX] = {
    fn_neg_integer, fn_b_not_integer};

ezInteger::ezInteger(int val) : ezValue(EZ_VALUE_TYPE_INTEGER), value(val) {
  m_size = sizeof(*this);
  m_fn_binary = fn_binary_integer;
  m_fn_unary = fn_unary_integer;
}

ezValue *ezInteger::condition(void) {
  return new ezCondition(value ? false : true, (value < 0) ? true : false,
                         false, false);
}

void ezInteger::set_operation(ezBinaryOperation op, ezValueType type,
                              fnBinaryOperation *fn) {
  if (!fn)
    throw runtime_error("null function is not allowed");
  fn_binary_integer[op][type] = fn;
}

void ezInteger::set_operation(ezUnaryOperation op, fnUnaryOperation *fn) {
  if (!fn)
    throw runtime_error("null function is not allowed");
  fn_unary_integer[op] = fn;
}
