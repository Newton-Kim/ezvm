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
#include "eaval.h"
#include "ezvm/ezfunc.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

ezString::ezString(const string val)
    : eaValue(EZ_VALUE_TYPE_STRING), value(val) {
  m_size = sizeof(*this) + val.size() + 1;
}

string ezString::to_string(void) {
  return value;
}

ezValue* ezString::add(ezValue* v, bool flip) {
  return new ezString(value + ((eaValue*)v)->to_string());
}

ezObject* ezString::compare(ezValue* v, bool flip) {
  return new ezCondition(v->id == EZ_VALUE_TYPE_STRING && value == ((eaValue*)v)->to_string(), false, false, false);
}

ezObject *ezString::condition(void) {
  return new ezCondition(value.empty(), false, false, false);
}

bool ezString::is_equal(ezValue*v) {
  if(EZ_VALUE_TYPE_STRING != v->id) return false;
  if(value != ((ezString*)v)->value) return false;
  return true;
}

void ezString::dump(ezFile &sink) {
    sink.print("\"%s\"\n", value.c_str());
}
