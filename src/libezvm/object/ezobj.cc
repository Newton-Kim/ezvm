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

#include "ezvm/ezobject.h"

ezObject::ezObject(ezObjectType tp) : type(tp) {}

const char* ezObject::type_string(void) {
  const char* ret = "unknown";
  switch(type) {
    case EZ_OBJECT_TYPE_NULL:
      ret = "nil";
      break;
    case EZ_OBJECT_TYPE_CONDITION:
      ret = "condition";
      break;
    case EZ_OBJECT_TYPE_FUNCTION:
      ret = "function";
      break;
    case EZ_OBJECT_TYPE_USER_DEFINED_FUNCTION:
      ret = "user defined function";
      break;
    case EZ_OBJECT_TYPE_HANDLE:
      ret = "handle";
      break;
    case EZ_OBJECT_TYPE_ARRAY:
      ret = "array";
      break;
    case EZ_OBJECT_TYPE_VALUE:
      ret = "value";
      break;
  }
  return ret;
}

void ezObject::dump(ezFile &sink) { sink.print("(unknown)"); }

ezHandle::ezHandle(unsigned int identifier)
    : ezObject(EZ_OBJECT_TYPE_HANDLE), id(identifier) {}

void ezHandle::dump(ezFile &sink) { sink.print("H(%d)\n", id); }
