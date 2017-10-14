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

static ezValue* fn_op_complex_error(ezValue* vl, ezValue* vr) {
  throw runtime_error("the operation with this type is not supported.");
}

static ezValue* fn_add_complex_integer(ezValue* vl, ezValue* vr) {
  complex<double> vp = ((ezComplex*)vl)->value;
  return new ezComplex(complex<double>(vp.real() + ((ezInteger*)vr)->value, vp.imag()));
}

static ezValue* fn_add_complex_float(ezValue* vl, ezValue* vr) {
  complex<double> vp = ((ezComplex*)vl)->value;
  return new ezComplex(complex<double>(vp.real() + ((ezFloat*)vr)->value, vp.imag()));
}

static ezValue* fn_add_complex_complex(ezValue* vl, ezValue* vr) {
  return new ezComplex(((ezComplex*)vl)->value + ((ezComplex*)vr)->value);
}

static ezValue* fn_sub_complex_integer(ezValue* vl, ezValue* vr) {
  complex<double> vp = ((ezComplex*)vl)->value;
  return new ezComplex(complex<double>(vp.real() - ((ezInteger*)vr)->value, vp.imag()));
}

static ezValue* fn_sub_complex_float(ezValue* vl, ezValue* vr) {
  complex<double> vp = ((ezComplex*)vl)->value;
  return new ezComplex(complex<double>(vp.real() - ((ezFloat*)vr)->value, vp.imag()));
}

static ezValue* fn_sub_complex_complex(ezValue* vl, ezValue* vr) {
  return new ezComplex(((ezComplex*)vl)->value - ((ezComplex*)vr)->value);
}

static ezValue* fn_mul_complex_integer(ezValue* vl, ezValue* vr) {
  complex<double> vp = ((ezComplex*)vl)->value;
  return new ezComplex(complex<double>(vp.real() * ((ezInteger*)vr)->value, vp.imag() * ((ezInteger*)vr)->value));
}

static ezValue* fn_mul_complex_float(ezValue* vl, ezValue* vr) {
  complex<double> vp = ((ezComplex*)vl)->value;
  return new ezComplex(complex<double>(vp.real() * ((ezFloat*)vr)->value, vp.imag() * ((ezFloat*)vr)->value));
}

static ezValue* fn_mul_complex_complex(ezValue* vl, ezValue* vr) {
  return new ezComplex(((ezComplex*)vl)->value * ((ezComplex*)vr)->value);
}

static ezValue* fn_div_complex_integer(ezValue* vl, ezValue* vr) {
  complex<double> vp = ((ezComplex*)vl)->value;
  return new ezComplex(complex<double>(vp.real() / ((ezInteger*)vr)->value, vp.imag() / ((ezInteger*)vr)->value));
}

static ezValue* fn_div_complex_float(ezValue* vl, ezValue* vr) {
  complex<double> vp = ((ezComplex*)vl)->value;
  return new ezComplex(complex<double>(vp.real() / ((ezFloat*)vr)->value, vp.imag() / ((ezFloat*)vr)->value));
}

static ezValue* fn_div_complex_complex(ezValue* vl, ezValue* vr) {
  return new ezComplex(((ezComplex*)vl)->value / ((ezComplex*)vr)->value);
}

static ezValue* fn_pow_complex_integer(ezValue* vl, ezValue* vr) {
  return new ezComplex(pow(((ezComplex*)vl)->value, ((ezInteger*)vr)->value));
}

static ezValue* fn_pow_complex_float(ezValue* vl, ezValue* vr) {
  return new ezComplex(pow(((ezComplex*)vl)->value, ((ezFloat*)vr)->value));
}

static ezValue* fn_pow_complex_complex(ezValue* vl, ezValue* vr) {
  return new ezComplex(pow(((ezComplex*)vl)->value, ((ezComplex*)vr)->value));
}

static ezValue* fn_cmp_complex_complex(ezValue* vl, ezValue* vr) {
  return new ezBool(((ezComplex*)vl)->value == ((ezComplex*)vr)->value);
}

static fnBinaryOperation* fn_add_complex[] = {
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_add_complex_integer,
  fn_add_complex_float,
  fn_add_complex_complex,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error
};

static fnBinaryOperation* fn_sub_complex[] = {
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_sub_complex_integer,
  fn_sub_complex_float,
  fn_sub_complex_complex,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error
};

static fnBinaryOperation* fn_mul_complex[] = {
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_mul_complex_integer,
  fn_mul_complex_float,
  fn_mul_complex_complex,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error
};

static fnBinaryOperation* fn_div_complex[] = {
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_div_complex_integer,
  fn_div_complex_float,
  fn_div_complex_complex,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error
};

static fnBinaryOperation* fn_pow_complex[] = {
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_pow_complex_integer,
  fn_pow_complex_float,
  fn_pow_complex_complex,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error
};

static fnBinaryOperation* fn_cmp_complex[] = {
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_cmp_complex_complex,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error,
  fn_op_complex_error
};

static ezValue* fn_neg_complex(ezValue* v) {
  complex<double> vr(-((ezComplex*)v)->value.real(), -((ezComplex*)v)->value.imag());
  return new ezComplex(vr);
}

ezComplex::ezComplex(complex<double> val)
    : ezValue(EZ_VALUE_TYPE_COMPLEX), value(val) {
  m_size = sizeof(*this);
  m_fn_add = fn_add_complex;
  m_fn_sub = fn_sub_complex;
  m_fn_mul = fn_mul_complex;
  m_fn_div = fn_div_complex;
  m_fn_pow = fn_pow_complex;
  m_fn_cmp = fn_cmp_complex;
  m_fn_neg = fn_neg_complex;
}

ezValue *ezComplex::condition(void) {
  return new ezCondition(abs(value) ? false : true,
                         (abs(value) < 0) ? true : false, false, false);
}

