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

static ezValue *fn_add_float_integer(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezFloat *)vl)->value + ((ezInteger *)vr)->value);
}

static ezValue *fn_add_float_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezFloat *)vl)->value + ((ezFloat *)vr)->value);
}

static ezValue *fn_add_float_complex(ezValue *vl, ezValue *vr) {
  complex<double> vp = ((ezComplex *)vr)->value;
  return new ezComplex(
      complex<double>(((ezFloat *)vl)->value + vp.real(), vp.imag()));
}

static ezValue *fn_sub_float_integer(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezFloat *)vl)->value - ((ezInteger *)vr)->value);
}

static ezValue *fn_sub_float_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezFloat *)vl)->value - ((ezFloat *)vr)->value);
}

static ezValue *fn_sub_float_complex(ezValue *vl, ezValue *vr) {
  complex<double> vp = ((ezComplex *)vr)->value;
  return new ezComplex(
      complex<double>(((ezFloat *)vl)->value - vp.real(), -vp.imag()));
}

static ezValue *fn_mul_float_integer(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezFloat *)vl)->value * ((ezInteger *)vr)->value);
}

static ezValue *fn_mul_float_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezFloat *)vl)->value * ((ezFloat *)vr)->value);
}

static ezValue *fn_mul_float_complex(ezValue *vl, ezValue *vr) {
  complex<double> vp = ((ezComplex *)vr)->value;
  return new ezComplex(complex<double>(((ezFloat *)vl)->value * vp.real(),
                                       ((ezFloat *)vl)->value * vp.imag()));
}

static ezValue *fn_div_float_integer(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezFloat *)vl)->value / ((ezInteger *)vr)->value);
}

static ezValue *fn_div_float_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(((ezFloat *)vl)->value / ((ezFloat *)vr)->value);
}

static ezValue *fn_div_float_complex(ezValue *vl, ezValue *vr) {
  complex<double> vp(((ezFloat *)vl)->value, 0);
  return new ezComplex(vp * ((ezComplex *)vr)->value);
}

static ezValue *fn_pow_float_integer(ezValue *vl, ezValue *vr) {
  return new ezFloat(pow(((ezFloat *)vl)->value, ((ezInteger *)vr)->value));
}

static ezValue *fn_pow_float_float(ezValue *vl, ezValue *vr) {
  return new ezFloat(pow(((ezFloat *)vl)->value, ((ezFloat *)vr)->value));
}

static ezValue *fn_pow_float_complex(ezValue *vl, ezValue *vr) {
  return new ezComplex(pow(((ezFloat *)vl)->value, ((ezComplex *)vr)->value));
}

static ezValue *fn_cmp_float_integer(ezValue *vl, ezValue *vr) {
  return new ezBool(((ezFloat *)vl)->value == ((ezInteger *)vr)->value);
}

static ezValue *fn_cmp_float_float(ezValue *vl, ezValue *vr) {
  return new ezBool(((ezFloat *)vl)->value == ((ezFloat *)vr)->value);
}

static fnBinaryOperation *fn_add_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_add_float_integer,    fn_add_float_float,      fn_add_float_complex,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_sub_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_sub_float_integer,    fn_sub_float_float,      fn_sub_float_complex,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_mul_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_mul_float_integer,    fn_mul_float_float,      fn_mul_float_complex,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_div_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_div_float_integer,    fn_div_float_float,      fn_div_float_complex,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_mod_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_pow_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_pow_float_integer,    fn_pow_float_float,      fn_pow_float_complex,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_cmp_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_cmp_float_integer,    fn_cmp_float_float,      fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_and_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_or_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_xor_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_lsl_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_lsr_float[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation **fn_binary_float[EZ_BINARY_OPERATION_MAX] = {
    fn_add_float,  fn_cmp_float,   fn_sub_float, fn_mul_float,
    fn_div_float,  fn_mod_float,   fn_pow_float, fn_b_and_float,
    fn_b_or_float, fn_b_xor_float, fn_lsl_float, fn_lsr_float};

static ezValue *fn_neg_float(ezValue *v) {
  return new ezFloat(((ezFloat *)v)->value);
}

static fnUnaryOperation *fn_unary_float[EZ_UNARY_OPERATION_MAX] = {
    fn_unary_generic_error, fn_neg_float};

ezFloat::ezFloat(double val) : ezValue(EZ_VALUE_TYPE_FLOAT), value(val) {
  m_size = sizeof(*this);
  m_fn_binary = fn_binary_float;
  m_fn_unary = fn_unary_float;
}

ezValue *ezFloat::condition(void) {
  return new ezCondition(value ? false : true, (value < 0) ? true : false,
                         false, false);
}

void ezFloat::set_operation(ezBinaryOperation op, ezValueType type,
                            fnBinaryOperation *fn) {
  if (!fn)
    throw runtime_error("null function is not allowed");
  fn_binary_float[op][type] = fn;
}

void ezFloat::set_operation(ezUnaryOperation op, fnUnaryOperation *fn) {
  if (!fn)
    throw runtime_error("null function is not allowed");
  fn_unary_float[op] = fn;
}
