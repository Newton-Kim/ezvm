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

static ezObject *fn_add_float_integer(ezValue *vl, ezValue *vr, bool flip) {
  return new ezFloat(((ezFloat *)vl)->value + ((ezInteger *)vr)->value);
}

static ezObject *fn_add_float_float(ezValue *vl, ezValue *vr, bool flip) {
  return new ezFloat(((ezFloat *)vl)->value + ((ezFloat *)vr)->value);
}

static ezObject *fn_sub_float_integer(ezValue *vl, ezValue *vr, bool flip) {
  double ret = (flip) ? ((ezInteger *)vr)->value - ((ezFloat *)vl)->value
                      : ((ezFloat *)vl)->value - ((ezInteger *)vr)->value;
  return new ezFloat(ret);
}

static ezObject *fn_sub_float_float(ezValue *vl, ezValue *vr, bool flip) {
  double ret = (flip) ? ((ezFloat *)vr)->value - ((ezFloat *)vl)->value
                      : ((ezFloat *)vl)->value - ((ezFloat *)vr)->value;
  return new ezFloat(ret);
}

static ezObject *fn_mul_float_integer(ezValue *vl, ezValue *vr, bool flip) {
  return new ezFloat(((ezFloat *)vl)->value * ((ezInteger *)vr)->value);
}

static ezObject *fn_mul_float_float(ezValue *vl, ezValue *vr, bool flip) {
  return new ezFloat(((ezFloat *)vl)->value * ((ezFloat *)vr)->value);
}

static ezObject *fn_div_float_integer(ezValue *vl, ezValue *vr, bool flip) {
  double ret = (flip) ? ((ezInteger *)vr)->value / ((ezFloat *)vl)->value
                      : ((ezFloat *)vl)->value / ((ezInteger *)vr)->value;
  return new ezFloat(ret);
}

static ezObject *fn_div_float_float(ezValue *vl, ezValue *vr, bool flip) {
  double ret = (flip) ? ((ezFloat *)vr)->value / ((ezFloat *)vl)->value
                      : ((ezFloat *)vl)->value / ((ezFloat *)vr)->value;
  return new ezFloat(ret);
}

static ezObject *fn_pow_float_integer(ezValue *vl, ezValue *vr, bool flip) {
  double ret = (flip) ? pow(((ezInteger *)vr)->value, ((ezFloat *)vl)->value)
                      : pow(((ezFloat *)vl)->value, ((ezInteger *)vr)->value);
  return new ezFloat(ret);
}

static ezObject *fn_pow_float_float(ezValue *vl, ezValue *vr, bool flip) {
  double ret = (flip) ? pow(((ezFloat *)vr)->value, ((ezFloat *)vl)->value)
                      : pow(((ezFloat *)vl)->value, ((ezFloat *)vr)->value);
  return new ezFloat(ret);
}

static ezObject *fn_cmp_float_integer(ezValue *vl, ezValue *vr, bool flip) {
  double ret = (flip) ? ((ezInteger *)vr)->value - ((ezFloat *)vl)->value
                      : ((ezFloat *)vl)->value - ((ezInteger *)vr)->value;
  return new ezCondition(ret == 0, ret < 0, false, false);
}

static ezObject *fn_cmp_float_float(ezValue *vl, ezValue *vr, bool flip) {
  double ret = (flip) ? ((ezFloat *)vr)->value - ((ezFloat *)vl)->value
                      : ((ezFloat *)vl)->value - ((ezFloat *)vr)->value;
  return new ezCondition(ret == 0, ret < 0, false, false);
}

static fnBinaryOperation *fn_add_float[EZ_VALUE_TYPE_FLOAT + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_add_float_integer,
    fn_add_float_float};

static fnBinaryOperation *fn_sub_float[EZ_VALUE_TYPE_FLOAT + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_sub_float_integer,
    fn_sub_float_float};

static fnBinaryOperation *fn_mul_float[EZ_VALUE_TYPE_FLOAT + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_mul_float_integer,
    fn_mul_float_float};

static fnBinaryOperation *fn_div_float[EZ_VALUE_TYPE_FLOAT + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_div_float_integer,
    fn_div_float_float};

static fnBinaryOperation *fn_default_float[EZ_VALUE_TYPE_FLOAT + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_pow_float[EZ_VALUE_TYPE_FLOAT + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_pow_float_integer,
    fn_pow_float_float};

static fnBinaryOperation *fn_cmp_float[EZ_VALUE_TYPE_FLOAT + 1] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_cmp_float_integer,
    fn_cmp_float_float};

static fnBinaryOperation **fn_binary_float[EZ_BINARY_OPERATION_MAX] = {
    fn_add_float,     fn_cmp_float,     fn_sub_float,     fn_mul_float,
    fn_div_float,     fn_default_float, fn_pow_float,     fn_default_float,
    fn_default_float, fn_default_float, fn_default_float, fn_default_float};

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

ezObject *ezFloat::condition(void) {
  return new ezCondition(value ? false : true, (value < 0) ? true : false,
                         false, false);
}

