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
#include "ezvm/ezfile.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

ezValue::ezValue(const unsigned int tp)
    : id(tp), ezObject(EZ_OBJECT_TYPE_VALUE) {
  m_size = sizeof(*this);
}
ezValue::~ezValue() {}

ezValue *ezValue::power(ezValue *arg) {
  stringstream ss;
  ss << "Type " << id << " cannot be powered";
  throw runtime_error(ss.str());
}

ezValue *ezValue::negate(void) {
  stringstream ss;
  ss << "Type " << id << " cannot be negated";
  throw runtime_error(ss.str());
}

ezValue *ezValue::bitwise_not(void) {
  stringstream ss;
  ss << "Type " << id << " cannot be bitwise not";
  throw runtime_error(ss.str());
}

ezValue *ezValue::lsl(ezValue *arg) {
  stringstream ss;
  ss << "Type " << id << " cannot be shifted left";
  throw runtime_error(ss.str());
}

ezValue *ezValue::lsr(ezValue *arg) {
  stringstream ss;
  ss << "Type " << id << " cannot be shifted right";
  throw runtime_error(ss.str());
}

ezObject *ezValue::compare(ezValue *arg) {
  stringstream ss;
  ss << "Type " << id << " cannot be compared";
  throw runtime_error(ss.str());
}

ezObject *ezValue::condition(void) {
  stringstream ss;
  ss << "Type " << id << " does not provide condition";
  throw runtime_error(ss.str());
}

bool ezValue::is_equal(ezValue *arg) {
  stringstream ss;
  ss << "Type " << id << " cannot test equality";
  throw runtime_error(ss.str());
}

ezObject *ezValue::get(ezValue *mbr) {
  stringstream ss;
  ss << "Type " << id << " is not container. So cannot get";
  throw runtime_error(ss.str());
}

ezObject *ezValue::get(vector<ezValue *> mbr) {
  stringstream ss;
  ss << "Type " << id << " is not container. So cannot get";
  throw runtime_error(ss.str());
}

void ezValue::set(ezValue *mbr, ezObject *val) {
  stringstream ss;
  ss << "Type " << id << " is not container. So cannot set";
  throw runtime_error(ss.str());
}

void ezValue::set(vector<ezValue *> mbr, ezObject *val) {
  stringstream ss;
  ss << "Type " << id << " is not container. So cannot set";
  throw runtime_error(ss.str());
}

ezValue *ezValue::duplicate(void) {
  stringstream ss;
  ss << "Type " << id << " does not be duplicated";
  throw runtime_error(ss.str());
}
