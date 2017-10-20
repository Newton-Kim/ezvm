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

static ezValue *fn_op_integer_error(ezValue *vl, ezValue *vr) {
  throw runtime_error("the operation with this type is not supported.");
}

static ezValue *fn_add_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value + ((ezInteger *)vr)->value);
}

static ezValue *fn_add_integer_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezInteger *)vl)->value + ((ezFloat *)vr)->value);
}

static ezValue *fn_add_integer_complex(ezValue *vl, ezValue *vr) {
  complex<double> vp = ((ezComplex *)vr)->value;
  return new ezComplex(
      complex<double>(((ezInteger *)vl)->value + vp.real(), vp.imag()));
}

static ezValue *fn_sub_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value - ((ezInteger *)vr)->value);
}

static ezValue *fn_sub_integer_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezInteger *)vl)->value - ((ezFloat *)vr)->value);
}

static ezValue *fn_sub_integer_complex(ezValue *vl, ezValue *vr) {
  complex<double> vp = ((ezComplex *)vr)->value;
  return new ezComplex(
      complex<double>(((ezInteger *)vl)->value - vp.real(), -vp.imag()));
}

static ezValue *fn_mul_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value * ((ezInteger *)vr)->value);
}

static ezValue *fn_mul_integer_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezInteger *)vl)->value * ((ezFloat *)vr)->value);
}

static ezValue *fn_mul_integer_complex(ezValue *vl, ezValue *vr) {
  complex<double> vp = ((ezComplex *)vr)->value;
  return new ezComplex(complex<double>(((ezInteger *)vl)->value * vp.real(),
                                       ((ezInteger *)vl)->value * vp.imag()));
}

static ezValue *fn_div_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value / ((ezInteger *)vr)->value);
}

static ezValue *fn_div_integer_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezInteger *)vl)->value / ((ezFloat *)vr)->value);
}

static ezValue *fn_div_integer_complex(ezValue *vl, ezValue *vr) {
  complex<double> vp(((ezInteger *)vl)->value, 0);
  return new ezComplex(vp * ((ezComplex *)vr)->value);
}

static ezValue *fn_mod_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value % ((ezInteger *)vr)->value);
}

static ezValue *fn_b_and_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value & ((ezInteger *)vr)->value);
}

static ezValue *fn_b_or_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value | ((ezInteger *)vr)->value);
}

static ezValue *fn_b_xor_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value ^ ((ezInteger *)vr)->value);
}

static ezValue *fn_lsl_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value << ((ezInteger *)vr)->value);
}

static ezValue *fn_lsr_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezInteger(((ezInteger *)vl)->value >> ((ezInteger *)vr)->value);
}

static ezValue *fn_pow_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezComplex(pow(((ezInteger *)vl)->value, ((ezInteger *)vr)->value));
}

static ezValue *fn_pow_integer_float(ezValue *vl, ezValue *vr) {
  return new ezComplex(pow(((ezInteger *)vl)->value, ((ezFloat *)vr)->value));
}

static ezValue *fn_pow_integer_complex(ezValue *vl, ezValue *vr) {
  return new ezComplex(pow(((ezInteger *)vl)->value, ((ezComplex *)vr)->value));
}

static ezValue *fn_cmp_integer_integer(ezValue *vl, ezValue *vr) {
  return new ezBool(((ezInteger *)vl)->value == ((ezInteger *)vr)->value);
}

static ezValue *fn_cmp_integer_float(ezValue *vl, ezValue *vr) {
  return new ezBool(((ezInteger *)vl)->value == ((ezFloat *)vr)->value);
}

static fnBinaryOperation *fn_add_integer[] = {
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_add_integer_integer, fn_add_integer_float, fn_add_integer_complex,
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_sub_integer[] = {
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_sub_integer_integer, fn_sub_integer_float, fn_sub_integer_complex,
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_mul_integer[] = {
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_mul_integer_integer, fn_mul_integer_float, fn_mul_integer_complex,
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_div_integer[] = {
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_div_integer_integer, fn_div_integer_float, fn_div_integer_complex,
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_mod_integer[] = {
    fn_op_integer_error,    fn_op_integer_error, fn_op_integer_error,
    fn_mod_integer_integer, fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error,    fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_pow_integer[] = {
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_pow_integer_integer, fn_pow_integer_float, fn_pow_integer_complex,
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_cmp_integer[] = {
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_cmp_integer_integer, fn_cmp_integer_float, fn_op_integer_error,
    fn_op_integer_error,    fn_op_integer_error,  fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_b_and_integer[] = {
    fn_op_integer_error,      fn_op_integer_error, fn_op_integer_error,
    fn_b_and_integer_integer, fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error,      fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_b_or_integer[] = {
    fn_op_integer_error,     fn_op_integer_error, fn_op_integer_error,
    fn_b_or_integer_integer, fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error,     fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_b_xor_integer[] = {
    fn_op_integer_error,      fn_op_integer_error, fn_op_integer_error,
    fn_b_xor_integer_integer, fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error,      fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_lsl_integer[] = {
    fn_op_integer_error,    fn_op_integer_error, fn_op_integer_error,
    fn_lsl_integer_integer, fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error,    fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error};

static fnBinaryOperation *fn_lsr_integer[] = {
    fn_op_integer_error,    fn_op_integer_error, fn_op_integer_error,
    fn_lsr_integer_integer, fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error,    fn_op_integer_error, fn_op_integer_error,
    fn_op_integer_error};

static ezValue *fn_b_not_integer(ezValue *v) {
  return new ezFloat(~((ezInteger *)v)->value);
}

static ezValue *fn_neg_integer(ezValue *v) {
  return new ezInteger(-((ezInteger *)v)->value);
}

ezInteger::ezInteger(int val) : ezValue(EZ_VALUE_TYPE_INTEGER), value(val) {
  m_size = sizeof(*this);
  m_fn_binary[EZ_BINARY_OPERATION_ADDITION] = fn_add_integer;
  m_fn_binary[EZ_BINARY_OPERATION_SUBTRACTION] = fn_sub_integer;
  m_fn_binary[EZ_BINARY_OPERATION_MULTIPLICATION] = fn_mul_integer;
  m_fn_binary[EZ_BINARY_OPERATION_DIVISION] = fn_div_integer;
  m_fn_binary[EZ_BINARY_OPERATION_MODULATION] = fn_mod_integer;
  m_fn_binary[EZ_BINARY_OPERATION_POW] = fn_pow_integer;
  m_fn_binary[EZ_BINARY_OPERATION_AND] = fn_b_and_integer;
  m_fn_binary[EZ_BINARY_OPERATION_OR] = fn_b_or_integer;
  m_fn_binary[EZ_BINARY_OPERATION_XOR] = fn_b_xor_integer;
  m_fn_unary[EZ_UNARY_OPERATION_NOT] = fn_b_not_integer;
  m_fn_binary[EZ_BINARY_OPERATION_COMPARISON] = fn_cmp_integer;
  m_fn_binary[EZ_BINARY_OPERATION_LSL] = fn_lsl_integer;
  m_fn_binary[EZ_BINARY_OPERATION_LSR] = fn_lsr_integer;
  m_fn_unary[EZ_UNARY_OPERATION_NEGATE] = fn_neg_integer;
}

ezValue *ezInteger::condition(void) {
  return new ezCondition(value ? false : true, (value < 0) ? true : false,
                         false, false);
}
