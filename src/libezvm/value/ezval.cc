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

ezValue *fn_binary_generic_error(ezValue *vl, ezValue *vr, bool flip) {
  throw runtime_error("the operation with given type is not supported");
}

ezValue *fn_unary_generic_error(ezValue *v) {
  throw runtime_error("the operation with given type is not supported");
}

static fnBinaryOperation *fn_add_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_sub_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_mul_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_div_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_mod_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_pow_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_cmp_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_and_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_or_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_b_xor_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_lsl_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation *fn_lsr_generic[EZ_VALUE_TYPE_MAX] = {
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error, fn_binary_generic_error, fn_binary_generic_error,
    fn_binary_generic_error};

static fnBinaryOperation **fn_binary_generic[EZ_BINARY_OPERATION_MAX] = {
    fn_add_generic,  fn_cmp_generic,   fn_sub_generic, fn_mul_generic,
    fn_div_generic,  fn_mod_generic,   fn_pow_generic, fn_b_and_generic,
    fn_b_or_generic, fn_b_xor_generic, fn_lsl_generic, fn_lsr_generic};

static fnUnaryOperation *fn_unary_generic[EZ_UNARY_OPERATION_MAX] = {
    fn_unary_generic_error, fn_unary_generic_error};

ezValue::ezValue(const ezValueType tp)
    : m_fn_binary(fn_binary_generic), m_fn_unary(fn_unary_generic), type(tp) {
  m_size = sizeof(*this);
}
ezValue::~ezValue() {}

ezValue *ezValue::condition(void) {
  throw runtime_error("not subject to a condition");
}

ezValue *ezValue::operate(ezBinaryOperation op, ezValue *v, bool flip) {
  return m_fn_binary[op][v->type](this, v, flip);
}

ezValue *ezValue::operate(ezUnaryOperation op) { return m_fn_unary[op](this); }
