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

ezValue::ezValue(const ezValueType tp) :
    m_fn_add(NULL),
    m_fn_sub(NULL),
    m_fn_mul(NULL),
    m_fn_div(NULL),
    m_fn_modulo(NULL),
    m_fn_pow(NULL),
    m_fn_b_and(NULL),
    m_fn_b_or(NULL),
    m_fn_b_xor(NULL),
    m_fn_b_not(NULL),
    m_fn_cmp(NULL),
    m_fn_lsl(NULL),
    m_fn_lsr(NULL),
    m_fn_neg(NULL),
    type(tp) { m_size = sizeof(*this); }
ezValue::~ezValue() {}

ezValue *ezValue::condition(void) {
  throw runtime_error("not subject to a condition");
}

ezValue *ezValue::add(ezValue *v) {
  if(!m_fn_add)
    throw runtime_error("add operation is not supported");
  return m_fn_add[v->type](this, v);
}

ezValue *ezValue::subtract(ezValue *v) {
  if(!m_fn_sub)
    throw runtime_error("sub operation is not supported");
  return m_fn_sub[v->type](this, v);
}

ezValue *ezValue::multiply(ezValue *v) {
  if(!m_fn_mul)
    throw runtime_error("mul operation is not supported");
  return m_fn_mul[v->type](this, v);
}

ezValue *ezValue::divide(ezValue *v) {
  if(!m_fn_div)
    throw runtime_error("div operation is not supported");
  return m_fn_div[v->type](this, v);
}

ezValue *ezValue::modulo(ezValue *v) {
  if(!m_fn_modulo)
    throw runtime_error("mod operation is not supported");
  return m_fn_modulo[v->type](this, v);
}

ezValue *ezValue::bitwise_and(ezValue *v) {
  if(!m_fn_b_and)
    throw runtime_error("and operation is not supported");
  return m_fn_b_and[v->type](this, v);
}

ezValue *ezValue::bitwise_or(ezValue *v) {
  if(!m_fn_b_or)
    throw runtime_error("or operation is not supported");
  return m_fn_b_or[v->type](this, v);
}

ezValue *ezValue::bitwise_xor(ezValue *v) {
  if(!m_fn_b_xor)
    throw runtime_error("xor operation is not supported");
  return m_fn_b_xor[v->type](this, v);
}

ezValue *ezValue::bitwise_not(void) {
  if(!m_fn_b_not)
    throw runtime_error("not operation is not supported");
  return m_fn_b_not(this);
}

ezValue *ezValue::compare(ezValue *v) {
  if(!m_fn_cmp)
    throw runtime_error("cmp operation is not supported");
  return m_fn_cmp[v->type](this, v);
}

ezValue *ezValue::lsl(ezValue *v) {
  if(!m_fn_lsl)
    throw runtime_error("lsl operation is not supported");
  return m_fn_lsl[v->type](this, v);
}

ezValue *ezValue::lsr(ezValue *v) {
  if(!m_fn_lsr)
    throw runtime_error("lsr operation is not supported");
  return m_fn_lsr[v->type](this, v);
}

ezValue *ezValue::negate(void) {
  if(!m_fn_neg)
    throw runtime_error("neg operation is not supported");
  return m_fn_neg(this);
}

ezValue *ezValue::powv(ezValue *v) {
  if(!m_fn_pow)
    throw runtime_error("pow operation is not supported");
  return m_fn_pow[v->type](this, v);
}
