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
#include "ezvm/ezfunc.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

static ezObject *fn_add_complex_integer(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> vp = ((ezComplex *)vl)->value;
  return new ezComplex(
      complex<double>(vp.real() + ((ezInteger *)vr)->value, vp.imag()));
}

static ezObject *fn_add_complex_float(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> vp = ((ezComplex *)vl)->value;
  return new ezComplex(
      complex<double>(vp.real() + ((ezFloat *)vr)->value, vp.imag()));
}

static ezObject *fn_add_complex_complex(ezValue *vl, ezValue *vr, bool flip) {
  return new ezComplex(((ezComplex *)vl)->value + ((ezComplex *)vr)->value);
}

static ezObject *fn_sub_complex_integer(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> vp = ((ezComplex *)vl)->value;
  complex<double> ret =
      (flip) ? complex<double>(((ezInteger *)vr)->value - vp.real(), -vp.imag())
             : complex<double>(vp.real() - ((ezInteger *)vr)->value, vp.imag());
  return new ezComplex(ret);
}

static ezObject *fn_sub_complex_float(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> vp = ((ezComplex *)vl)->value;
  complex<double> ret =
      (flip) ? complex<double>(((ezFloat *)vr)->value - vp.real(), -vp.imag())
             : complex<double>(vp.real() - ((ezFloat *)vr)->value, vp.imag());
  return new ezComplex(ret);
}

static ezObject *fn_sub_complex_complex(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> ret =
      (flip)
          ? complex<double>(((ezComplex *)vr)->value - ((ezComplex *)vl)->value)
          : complex<double>(((ezComplex *)vl)->value -
                            ((ezComplex *)vr)->value);
  return new ezComplex(ret);
}

static ezObject *fn_mul_complex_integer(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> vp = ((ezComplex *)vl)->value;
  return new ezComplex(complex<double>(vp.real() * ((ezInteger *)vr)->value,
                                       vp.imag() * ((ezInteger *)vr)->value));
}

static ezObject *fn_mul_complex_float(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> vp = ((ezComplex *)vl)->value;
  return new ezComplex(complex<double>(vp.real() * ((ezFloat *)vr)->value,
                                       vp.imag() * ((ezFloat *)vr)->value));
}

static ezObject *fn_mul_complex_complex(ezValue *vl, ezValue *vr, bool flip) {
  return new ezComplex(((ezComplex *)vl)->value * ((ezComplex *)vr)->value);
}

static ezObject *fn_div_complex_integer(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> vp = ((ezComplex *)vl)->value;
  complex<double> ret =
      (flip) ? complex<double>(vp.real() * ((ezInteger *)vr)->value / abs(vp),
                               -vp.imag() * ((ezInteger *)vr)->value / abs(vp))
             : complex<double>(vp.real() / ((ezInteger *)vr)->value,
                               vp.imag() / ((ezInteger *)vr)->value);
  return new ezComplex(ret);
}

static ezObject *fn_div_complex_float(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> vp = ((ezComplex *)vl)->value;
  complex<double> ret =
      (flip) ? complex<double>(vp.real() * ((ezFloat *)vr)->value / abs(vp),
                               -vp.imag() * ((ezFloat *)vr)->value / abs(vp))
             : complex<double>(vp.real() / ((ezFloat *)vr)->value,
                               vp.imag() / ((ezFloat *)vr)->value);
  return new ezComplex(ret);
}

static ezObject *fn_div_complex_complex(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> ret =
      (flip) ? ((ezComplex *)vr)->value / ((ezComplex *)vl)->value
             : ((ezComplex *)vl)->value / ((ezComplex *)vr)->value;
  return new ezComplex(ret);
}

static ezObject *fn_pow_complex_integer(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> ret =
      (flip) ? pow(((ezInteger *)vr)->value, ((ezComplex *)vl)->value)
             : pow(((ezComplex *)vl)->value, ((ezInteger *)vr)->value);
  return new ezComplex(ret);
}

static ezObject *fn_pow_complex_float(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> ret =
      (flip) ? pow(((ezFloat *)vr)->value, ((ezComplex *)vl)->value)
             : pow(((ezComplex *)vl)->value, ((ezFloat *)vr)->value);
  return new ezComplex(ret);
}

static ezObject *fn_pow_complex_complex(ezValue *vl, ezValue *vr, bool flip) {
  complex<double> ret =
      (flip) ? pow(((ezComplex *)vr)->value, ((ezComplex *)vl)->value)
             : pow(((ezComplex *)vl)->value, ((ezComplex *)vr)->value);
  return new ezComplex(ret);
}

static ezObject *fn_cmp_complex_complex(ezValue *vl, ezValue *vr, bool flip) {
  return new ezCondition(((ezComplex *)vl)->value == ((ezComplex *)vr)->value,
                         false, false, false);
}

static fnBinaryOperation *fn_add_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_add_complex_integer,
    fn_add_complex_float, fn_add_complex_complex};

static fnBinaryOperation *fn_sub_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_sub_complex_integer,
    fn_sub_complex_float, fn_sub_complex_complex};

static fnBinaryOperation *fn_mul_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_mul_complex_integer,
    fn_mul_complex_float, fn_mul_complex_complex};

static fnBinaryOperation *fn_div_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_div_complex_integer,
    fn_div_complex_float, fn_div_complex_complex};

static fnBinaryOperation *fn_mod_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error};

static fnBinaryOperation *fn_pow_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_pow_complex_integer,
    fn_pow_complex_float, fn_pow_complex_complex};

static fnBinaryOperation *fn_cmp_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_cmp_complex_complex};

static fnBinaryOperation *fn_b_and_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error};

static fnBinaryOperation *fn_b_or_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error};

static fnBinaryOperation *fn_b_xor_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error};

static fnBinaryOperation *fn_lsl_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error};

static fnBinaryOperation *fn_lsr_complex[EZ_VALUE_TYPE_COMPLEX + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error};

static fnBinaryOperation **fn_binary_complex[EZ_BINARY_OPERATION_MAX] = {
    fn_add_complex,  fn_cmp_complex,   fn_sub_complex, fn_mul_complex,
    fn_div_complex,  fn_mod_complex,   fn_pow_complex, fn_b_and_complex,
    fn_b_or_complex, fn_b_xor_complex, fn_lsl_complex, fn_lsr_complex};

static ezValue *fn_neg_complex(ezValue *v) {
  complex<double> vr(-((ezComplex *)v)->value.real(),
                     -((ezComplex *)v)->value.imag());
  return new ezComplex(vr);
}

static fnUnaryOperation *fn_unary_complex[EZ_UNARY_OPERATION_MAX] = {
    fn_unary_generic_error, fn_neg_complex};

ezComplex::ezComplex(complex<double> val)
    : ezValue(EZ_VALUE_TYPE_COMPLEX), value(val) {
  m_size = sizeof(*this);
  m_fn_binary = fn_binary_complex;
  m_fn_unary = fn_unary_complex;
}

ezObject *ezComplex::condition(void) {
  return new ezCondition(abs(value) ? false : true,
                         (abs(value) < 0) ? true : false, false, false);
}

