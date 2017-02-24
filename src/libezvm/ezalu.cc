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
#include "ezvm/ezalu.h"

static ezValueType defaultCoercTable
    [EZ_COERC_OPERATION_MAX][EZ_VALUE_TYPE_MAX][EZ_VALUE_TYPE_MAX] = {
        // EZ_COERC_OPERATION_ADDITION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_STRING,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_COMPARISON,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_STRING, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_SUBTRACTION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_MULTIPLICATION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_DIVISION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_MODULATION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_AND,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_OR,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_XOR
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}, // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX} // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        }};

ezALU::ezALU(ezGC &gc) : m_gc(gc) {}

typedef ezValue *(*ARITHEMATIC_DUO)(ezGC &gc, ezValue *larg, ezValue *rarg);
typedef ezValue *(*ARITHEMATIC_UNI)(ezGC &gc, ezValue *arg);

static ezValue *addd_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for addition");
  return NULL;
}

static ezValue *addd_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezInteger(larg->to_integer() + rarg->to_integer()));
}

static ezValue *addd_float(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezFloat(larg->to_float() + rarg->to_float()));
}

static ezValue *addd_string(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezString(larg->to_string() + rarg->to_string()));
}

ARITHEMATIC_DUO addd[EZ_VALUE_TYPE_MAX] = {
    addd_default, // EZ_VALUE_TYPE_NULL = 0,
    addd_default, // EZ_VALUE_TYPE_CONDITION,
    addd_default, // EZ_VALUE_TYPE_BOOL,
    addd_integer, // EZ_VALUE_TYPE_INTEGER,
    addd_float,   // EZ_VALUE_TYPE_FLOAT,
    addd_string,  // EZ_VALUE_TYPE_STRING,
    addd_default, // EZ_VALUE_TYPE_CAROUSEL,
    addd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::add(ezValue *larg, ezValue *rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_ADDITION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do addition");
  return addd[type](m_gc, larg, rarg);
}

static ezValue *cmp_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for comparison");
  return NULL;
}

static ezValue *cmp_bool(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add((ezGCObject *)new ezCondition(
      !(larg->to_bool() ^ rarg->to_bool()), false, false, false));
}

static ezValue *cmp_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  int v = larg->to_integer() - rarg->to_integer();
  return (ezValue *)gc.add((ezGCObject *)new ezCondition(
      (v ? false : true), ((v < 0) ? true : false), false, false));
}

static ezValue *cmp_float(ezGC &gc, ezValue *larg, ezValue *rarg) {
  int v = larg->to_float() - rarg->to_float();
  return (ezValue *)gc.add((ezGCObject *)new ezCondition(
      (v ? false : true), ((v < 0) ? true : false), false, false));
}

static ezValue *cmp_string(ezGC &gc, ezValue *larg, ezValue *rarg) {
  bool eq = (larg->to_string() == rarg->to_string());
  int sz = larg->to_string().size() - rarg->to_string().size();
  return (ezValue *)gc.add((ezGCObject *)new ezCondition(
      eq, ((sz < 0) ? true : false), false, false));
}

ARITHEMATIC_DUO cmpf[EZ_VALUE_TYPE_MAX] = {
    cmp_default, // EZ_VALUE_TYPE_NULL = 0,
    cmp_default, // EZ_VALUE_TYPE_CONDITION,
    cmp_bool,    // EZ_VALUE_TYPE_BOOL,
    cmp_integer, // EZ_VALUE_TYPE_INTEGER,
    cmp_float,   // EZ_VALUE_TYPE_FLOAT,
    cmp_string,  // EZ_VALUE_TYPE_STRING,
    cmp_default, // EZ_VALUE_TYPE_CAROUSEL,
    cmp_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::cmp(ezValue *larg, ezValue *rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_COMPARISON][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to compare");
  return cmpf[type](m_gc, larg, rarg);
}

static ezValue *subd_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for subtraction");
  return NULL;
}

static ezValue *subd_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezInteger(larg->to_integer() - rarg->to_integer()));
}

static ezValue *subd_float(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezFloat(larg->to_float() - rarg->to_float()));
}

ARITHEMATIC_DUO subd[EZ_VALUE_TYPE_MAX] = {
    subd_default, // EZ_VALUE_TYPE_NULL = 0,
    subd_default, // EZ_VALUE_TYPE_CONDITION,
    subd_default, // EZ_VALUE_TYPE_BOOL,
    subd_integer, // EZ_VALUE_TYPE_INTEGER,
    subd_float,   // EZ_VALUE_TYPE_FLOAT,
    subd_default, // EZ_VALUE_TYPE_STRING,
    subd_default, // EZ_VALUE_TYPE_CAROUSEL,
    subd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::sub(ezValue *larg, ezValue *rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_SUBTRACTION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do subtraction");
  return subd[type](m_gc, larg, rarg);
}

static ezValue *modd_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for modulation");
  return NULL;
}

static ezValue *modd_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezInteger(larg->to_integer() % rarg->to_integer()));
}

static ezValue *modd_float(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezFloat(larg->to_integer() % rarg->to_integer()));
}

ARITHEMATIC_DUO modd[EZ_VALUE_TYPE_MAX] = {
    modd_default, // EZ_VALUE_TYPE_NULL = 0,
    modd_default, // EZ_VALUE_TYPE_CONDITION,
    modd_default, // EZ_VALUE_TYPE_BOOL,
    modd_integer, // EZ_VALUE_TYPE_INTEGER,
    modd_float,   // EZ_VALUE_TYPE_FLOAT,
    modd_default, // EZ_VALUE_TYPE_STRING,
    modd_default, // EZ_VALUE_TYPE_CAROUSEL,
    modd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::mod(ezValue *larg, ezValue *rarg) {
  ezValueType type = defaultCoercTable[EZ_COERC_OPERATION_MULTIPLICATION]
                                      [larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do modulation");
  return modd[type](m_gc, larg, rarg);
}

static ezValue *muld_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for multiplication");
  return NULL;
}

static ezValue *muld_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezInteger(larg->to_integer() * rarg->to_integer()));
}

static ezValue *muld_float(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezFloat(larg->to_float() * rarg->to_float()));
}

ARITHEMATIC_DUO muld[EZ_VALUE_TYPE_MAX] = {
    muld_default, // EZ_VALUE_TYPE_NULL = 0,
    muld_default, // EZ_VALUE_TYPE_CONDITION,
    muld_default, // EZ_VALUE_TYPE_BOOL,
    muld_integer, // EZ_VALUE_TYPE_INTEGER,
    muld_float,   // EZ_VALUE_TYPE_FLOAT,
    muld_default, // EZ_VALUE_TYPE_STRING,
    muld_default, // EZ_VALUE_TYPE_CAROUSEL,
    muld_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::mul(ezValue *larg, ezValue *rarg) {
  ezValueType type = defaultCoercTable[EZ_COERC_OPERATION_MULTIPLICATION]
                                      [larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do multiplication");
  return muld[type](m_gc, larg, rarg);
}

static ezValue *divd_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for division");
  return NULL;
}

static ezValue *divd_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezInteger(larg->to_integer() / rarg->to_integer()));
}

static ezValue *divd_float(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezFloat(larg->to_float() / rarg->to_float()));
}

ARITHEMATIC_DUO divd[EZ_VALUE_TYPE_MAX] = {
    divd_default, // EZ_VALUE_TYPE_NULL = 0,
    divd_default, // EZ_VALUE_TYPE_CONDITION,
    divd_default, // EZ_VALUE_TYPE_BOOL,
    divd_integer, // EZ_VALUE_TYPE_INTEGER,
    divd_float,   // EZ_VALUE_TYPE_FLOAT,
    divd_default, // EZ_VALUE_TYPE_STRING,
    divd_default, // EZ_VALUE_TYPE_CAROUSEL,
    divd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::div(ezValue *larg, ezValue *rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_DIVISION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do division");
  return divd[type](m_gc, larg, rarg);
}

ezValue *ezALU::neg(ezValue *arg) {
  ezValue *ret = NULL;
  int ival = arg->to_integer();
  switch (arg->type) {
  case EZ_VALUE_TYPE_INTEGER:
    ret = new ezInteger(-ival);
    break;
  default:
    throw runtime_error("unable to negate");
    break;
  }
  return ret;
}

static ezValue *not_default(ezGC &gc, ezValue *arg) {
  throw runtime_error("invalid type for NOT");
  return NULL;
}

static ezValue *not_bool(ezGC &gc, ezValue *arg) {
  return new ezBool(!arg->to_bool());
}

static ezValue *not_integer(ezGC &gc, ezValue *arg) {
  return (ezValue *)gc.add((ezGCObject *)new ezInteger(~arg->to_integer()));
}

ARITHEMATIC_UNI notf[EZ_VALUE_TYPE_MAX] = {
    not_default, // EZ_VALUE_TYPE_NULL = 0,
    not_default, // EZ_VALUE_TYPE_CONDITION,
    not_bool,    // EZ_VALUE_TYPE_BOOL,
    not_integer, // EZ_VALUE_TYPE_INTEGER,
    not_default, // EZ_VALUE_TYPE_FLOAT,
    not_default, // EZ_VALUE_TYPE_STRING,
    not_default, // EZ_VALUE_TYPE_CAROUSEL,
    not_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::bitwise_not(ezValue *arg) { return notf[arg->type](m_gc, arg); }

static ezValue *andd_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for AND");
  return NULL;
}

static ezValue *andd_bool(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezBool(larg->to_bool() & rarg->to_bool()));
}

static ezValue *andd_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezInteger(larg->to_integer() & rarg->to_integer()));
}

ARITHEMATIC_DUO andd[EZ_VALUE_TYPE_MAX] = {
    andd_default, // EZ_VALUE_TYPE_NULL = 0,
    andd_default, // EZ_VALUE_TYPE_CONDITION,
    andd_bool,    // EZ_VALUE_TYPE_BOOL,
    andd_integer, // EZ_VALUE_TYPE_INTEGER,
    andd_default, // EZ_VALUE_TYPE_FLOAT,
    andd_default, // EZ_VALUE_TYPE_STRING,
    andd_default, // EZ_VALUE_TYPE_CAROUSEL,
    andd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::bitwise_and(ezValue *larg, ezValue *rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_AND][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do AND");
  return andd[type](m_gc, larg, rarg);
}

static ezValue *ord_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for OR");
  return NULL;
}

static ezValue *ord_bool(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezBool(larg->to_bool() | rarg->to_bool()));
}

static ezValue *ord_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezInteger(larg->to_integer() | rarg->to_integer()));
}

ARITHEMATIC_DUO ord[EZ_VALUE_TYPE_MAX] = {
    ord_default, // EZ_VALUE_TYPE_NULL = 0,
    ord_default, // EZ_VALUE_TYPE_CONDITION,
    ord_bool,    // EZ_VALUE_TYPE_BOOL,
    ord_integer, // EZ_VALUE_TYPE_INTEGER,
    ord_default, // EZ_VALUE_TYPE_FLOAT,
    ord_default, // EZ_VALUE_TYPE_STRING,
    ord_default, // EZ_VALUE_TYPE_CAROUSEL,
    ord_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::bitwise_or(ezValue *larg, ezValue *rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_OR][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do OR");
  return ord[type](m_gc, larg, rarg);
}

static ezValue *xord_default(ezGC &gc, ezValue *larg, ezValue *rarg) {
  throw runtime_error("invalid type for XOR");
  return NULL;
}

static ezValue *xord_bool(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezBool(larg->to_bool() ^ rarg->to_bool()));
}

static ezValue *xord_integer(ezGC &gc, ezValue *larg, ezValue *rarg) {
  return (ezValue *)gc.add(
      (ezGCObject *)new ezInteger(larg->to_integer() ^ rarg->to_integer()));
}

ARITHEMATIC_DUO xord[EZ_VALUE_TYPE_MAX] = {
    xord_default, // EZ_VALUE_TYPE_NULL = 0,
    xord_default, // EZ_VALUE_TYPE_CONDITION,
    xord_bool,    // EZ_VALUE_TYPE_BOOL,
    xord_integer, // EZ_VALUE_TYPE_INTEGER,
    xord_default, // EZ_VALUE_TYPE_FLOAT,
    xord_default, // EZ_VALUE_TYPE_STRING,
    xord_default, // EZ_VALUE_TYPE_CAROUSEL,
    xord_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue *ezALU::bitwise_xor(ezValue *larg, ezValue *rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_XOR][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do XOR");
  return xord[type](m_gc, larg, rarg);
}
