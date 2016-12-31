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
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,    EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_STRING,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_COMPARISON,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,    EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_STRING, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_SUBTRACTION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_MULTIPLICATION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_DIVISION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_MODULATION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_AND,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_OR,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_XOR
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_FLOAT, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_FLOAT,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        }};

ezALU::ezALU(ezGC<ezValue>& gc):m_gc(gc) {}

typedef ezValue* (*ARITHEMATIC_V)(ezGC<ezValue>& gc, vector<ezValue*>& args);
typedef ezValue* (*ARITHEMATIC_DUO)(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg);
typedef ezValue* (*ARITHEMATIC_UNI)(ezGC<ezValue>& gc, ezValue* arg);

static ezValue* addv_default(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  throw runtime_error("invalid type for addition");
  return NULL;
}

static ezValue* addv_integer(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret += args[i]->to_integer();
  return gc.add(new ezInteger(iret));
}

static ezValue* addv_float(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  double iret = args[0]->to_float();
  for (size_t i = 1; i < args.size(); i++) iret += args[i]->to_float();
  return gc.add(new ezFloat(iret));
}

static ezValue* addv_string(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  string sret = args[0]->to_string();
  for (size_t i = 1; i < args.size(); i++) sret += args[i]->to_string();
  return gc.add(new ezString(sret));
}

ARITHEMATIC_V addv[EZ_VALUE_TYPE_MAX] = {
    addv_default,  // EZ_VALUE_TYPE_NULL = 0,
    addv_default,  // EZ_VALUE_TYPE_CONDITION,
    addv_default,  // EZ_VALUE_TYPE_BOOL,
    addv_integer,  // EZ_VALUE_TYPE_INTEGER,
    addv_float,    // EZ_VALUE_TYPE_FLOAT,
    addv_string,   // EZ_VALUE_TYPE_STRING,
    addv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    addv_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::add(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = defaultCoercTable[EZ_COERC_OPERATION_ADDITION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do addition");
  }
  return addv[type](m_gc, args);
}

static ezValue* addd_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for addition");
  return NULL;
}

static ezValue* addd_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezInteger(larg->to_integer() + rarg->to_integer()));
}

static ezValue* addd_float(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezFloat(larg->to_float() + rarg->to_float()));
}

static ezValue* addd_string(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezString(larg->to_string() + rarg->to_string()));
}

ARITHEMATIC_DUO addd[EZ_VALUE_TYPE_MAX] = {
    addd_default,  // EZ_VALUE_TYPE_NULL = 0,
    addd_default,  // EZ_VALUE_TYPE_CONDITION,
    addd_default,  // EZ_VALUE_TYPE_BOOL,
    addd_integer,  // EZ_VALUE_TYPE_INTEGER,
    addd_float,    // EZ_VALUE_TYPE_FLOAT,
    addd_string,   // EZ_VALUE_TYPE_STRING,
    addd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    addd_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::add(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_ADDITION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do addition");
  return addd[type](m_gc, larg, rarg);
}

static ezValue* cmp_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for comparison");
  return NULL;
}

static ezValue* cmp_bool(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezCondition(!(larg->to_bool() ^ rarg->to_bool()), false, false,
                         false));
}

static ezValue* cmp_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  int v = larg->to_integer() - rarg->to_integer();
  return gc.add(new ezCondition((v ? false : true), ((v < 0) ? true : false), false,
                         false));
}

static ezValue* cmp_float(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  int v = larg->to_float() - rarg->to_float();
  return gc.add(new ezCondition((v ? false : true), ((v < 0) ? true : false), false,
                         false));
}

static ezValue* cmp_string(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  bool eq = (larg->to_string() == rarg->to_string());
  int sz = larg->to_string().size() - rarg->to_string().size();
  return gc.add(new ezCondition(eq, ((sz < 0) ? true : false), false, false));
}

ARITHEMATIC_DUO cmpf[EZ_VALUE_TYPE_MAX] = {
    cmp_default,  // EZ_VALUE_TYPE_NULL = 0,
    cmp_default,  // EZ_VALUE_TYPE_CONDITION,
    cmp_bool,     // EZ_VALUE_TYPE_BOOL,
    cmp_integer,  // EZ_VALUE_TYPE_INTEGER,
    cmp_float,    // EZ_VALUE_TYPE_FLOAT,
    cmp_string,   // EZ_VALUE_TYPE_STRING,
    cmp_default,  // EZ_VALUE_TYPE_CAROUSEL,
    cmp_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::cmp(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_COMPARISON][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to compare");
  return cmpf[type](m_gc, larg, rarg);
}

static ezValue* subv_default(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  throw runtime_error("invalid type for subtraction");
  return NULL;
}

static ezValue* subv_integer(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret -= args[i]->to_integer();
  return gc.add(new ezInteger(iret));
}

static ezValue* subv_float(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  double iret = args[0]->to_float();
  for (size_t i = 1; i < args.size(); i++) iret -= args[i]->to_float();
  return gc.add(new ezFloat(iret));
}

ARITHEMATIC_V subv[EZ_VALUE_TYPE_MAX] = {
    subv_default,  // EZ_VALUE_TYPE_NULL = 0,
    subv_default,  // EZ_VALUE_TYPE_CONDITION,
    subv_default,  // EZ_VALUE_TYPE_BOOL,
    subv_integer,  // EZ_VALUE_TYPE_INTEGER,
    subv_float,    // EZ_VALUE_TYPE_FLOAT,
    subv_default,  // EZ_VALUE_TYPE_STRING,
    subv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    subv_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::sub(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = defaultCoercTable[EZ_COERC_OPERATION_SUBTRACTION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX)
      throw runtime_error("unable to do subtraction");
  }
  return subv[type](m_gc, args);
}

static ezValue* subd_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for subtraction");
  return NULL;
}

static ezValue* subd_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezInteger(larg->to_integer() - rarg->to_integer()));
}

static ezValue* subd_float(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezFloat(larg->to_float() - rarg->to_float()));
}

ARITHEMATIC_DUO subd[EZ_VALUE_TYPE_MAX] = {
    subd_default,  // EZ_VALUE_TYPE_NULL = 0,
    subd_default,  // EZ_VALUE_TYPE_CONDITION,
    subd_default,  // EZ_VALUE_TYPE_BOOL,
    subd_integer,  // EZ_VALUE_TYPE_INTEGER,
    subd_float,    // EZ_VALUE_TYPE_FLOAT,
    subd_default,  // EZ_VALUE_TYPE_STRING,
    subd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    subd_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::sub(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_SUBTRACTION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do subtraction");
  return subd[type](m_gc, larg, rarg);
}

static ezValue* modv_default(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  throw runtime_error("invalid type for modulation");
  return NULL;
}

static ezValue* modv_integer(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret %= args[i]->to_integer();
  return gc.add(new ezInteger(iret));
}

ARITHEMATIC_V modv[EZ_VALUE_TYPE_MAX] = {
    modv_default,  // EZ_VALUE_TYPE_NULL = 0,
    modv_default,  // EZ_VALUE_TYPE_CONDITION,
    modv_default,  // EZ_VALUE_TYPE_BOOL,
    modv_integer,  // EZ_VALUE_TYPE_INTEGER,
    modv_default,  // EZ_VALUE_TYPE_FLOAT,
    modv_default,  // EZ_VALUE_TYPE_STRING,
    modv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    modv_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::mod(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type =
        defaultCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX)
      throw runtime_error("unable to do modulation");
  }
  return modv[type](m_gc, args);
}

static ezValue* modd_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for modulation");
  return NULL;
}

static ezValue* modd_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezInteger(larg->to_integer() % rarg->to_integer()));
}

static ezValue* modd_float(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezFloat(larg->to_integer() % rarg->to_integer()));
}

ARITHEMATIC_DUO modd[EZ_VALUE_TYPE_MAX] = {
    modd_default,  // EZ_VALUE_TYPE_NULL = 0,
    modd_default,  // EZ_VALUE_TYPE_CONDITION,
    modd_default,  // EZ_VALUE_TYPE_BOOL,
    modd_integer,  // EZ_VALUE_TYPE_INTEGER,
    modd_float,    // EZ_VALUE_TYPE_FLOAT,
    modd_default,  // EZ_VALUE_TYPE_STRING,
    modd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    modd_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::mod(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do modulation");
  return modd[type](m_gc, larg, rarg);
}

static ezValue* mulv_default(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  throw runtime_error("invalid type for multiplication");
  return NULL;
}

static ezValue* mulv_integer(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret *= args[i]->to_integer();
  return gc.add(new ezInteger(iret));
}

static ezValue* mulv_float(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  double iret = args[0]->to_float();
  for (size_t i = 1; i < args.size(); i++) iret *= args[i]->to_float();
  return gc.add(new ezFloat(iret));
}

ARITHEMATIC_V mulv[EZ_VALUE_TYPE_MAX] = {
    mulv_default,  // EZ_VALUE_TYPE_NULL = 0,
    mulv_default,  // EZ_VALUE_TYPE_CONDITION,
    mulv_default,  // EZ_VALUE_TYPE_BOOL,
    mulv_integer,  // EZ_VALUE_TYPE_INTEGER,
    mulv_float,    // EZ_VALUE_TYPE_FLOAT,
    mulv_default,  // EZ_VALUE_TYPE_STRING,
    mulv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    mulv_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::mul(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type =
        defaultCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX)
      throw runtime_error("unable to do multiplication");
  }
  return mulv[type](m_gc, args);
}

static ezValue* muld_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for multiplication");
  return NULL;
}

static ezValue* muld_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezInteger(larg->to_integer() * rarg->to_integer()));
}

static ezValue* muld_float(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezFloat(larg->to_float() * rarg->to_float()));
}

ARITHEMATIC_DUO muld[EZ_VALUE_TYPE_MAX] = {
    muld_default,  // EZ_VALUE_TYPE_NULL = 0,
    muld_default,  // EZ_VALUE_TYPE_CONDITION,
    muld_default,  // EZ_VALUE_TYPE_BOOL,
    muld_integer,  // EZ_VALUE_TYPE_INTEGER,
    muld_float,    // EZ_VALUE_TYPE_FLOAT,
    muld_default,  // EZ_VALUE_TYPE_STRING,
    muld_default,  // EZ_VALUE_TYPE_CAROUSEL,
    muld_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::mul(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX)
    throw runtime_error("unable to do multiplication");
  return muld[type](m_gc, larg, rarg);
}

static ezValue* divv_default(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  throw runtime_error("invalid type for division");
  return NULL;
}

static ezValue* divv_integer(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret /= args[i]->to_integer();
  return gc.add(new ezInteger(iret));
}

static ezValue* divv_float(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  double iret = args[0]->to_float();
  for (size_t i = 1; i < args.size(); i++) iret /= args[i]->to_float();
  return gc.add(new ezFloat(iret));
}

ARITHEMATIC_V divv[EZ_VALUE_TYPE_MAX] = {
    divv_default,  // EZ_VALUE_TYPE_NULL = 0,
    divv_default,  // EZ_VALUE_TYPE_CONDITION,
    divv_default,  // EZ_VALUE_TYPE_BOOL,
    divv_integer,  // EZ_VALUE_TYPE_INTEGER,
    divv_float,    // EZ_VALUE_TYPE_FLOAT,
    divv_default,  // EZ_VALUE_TYPE_STRING,
    divv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    divv_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::div(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = defaultCoercTable[EZ_COERC_OPERATION_DIVISION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do division");
  }
  return divv[type](m_gc, args);
}

static ezValue* divd_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for division");
  return NULL;
}

static ezValue* divd_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezInteger(larg->to_integer() / rarg->to_integer()));
}

static ezValue* divd_float(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezFloat(larg->to_float() / rarg->to_float()));
}

ARITHEMATIC_DUO divd[EZ_VALUE_TYPE_MAX] = {
    divd_default,  // EZ_VALUE_TYPE_NULL = 0,
    divd_default,  // EZ_VALUE_TYPE_CONDITION,
    divd_default,  // EZ_VALUE_TYPE_BOOL,
    divd_integer,  // EZ_VALUE_TYPE_INTEGER,
    divd_float,    // EZ_VALUE_TYPE_FLOAT,
    divd_default,  // EZ_VALUE_TYPE_STRING,
    divd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    divd_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::div(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_DIVISION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do division");
  return divd[type](m_gc, larg, rarg);
}

ezValue* ezALU::neg(ezValue* arg) {
  ezValue* ret = NULL;
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

static ezValue* not_default(ezGC<ezValue>& gc, ezValue* arg) {
  throw runtime_error("invalid type for NOT");
  return NULL;
}

static ezValue* not_bool(ezGC<ezValue>& gc, ezValue* arg) { return new ezBool(!arg->to_bool()); }

static ezValue* not_integer(ezGC<ezValue>& gc, ezValue* arg) {
  return gc.add(new ezInteger(~arg->to_integer()));
}

ARITHEMATIC_UNI notf[EZ_VALUE_TYPE_MAX] = {
    not_default,  // EZ_VALUE_TYPE_NULL = 0,
    not_default,  // EZ_VALUE_TYPE_CONDITION,
    not_bool,     // EZ_VALUE_TYPE_BOOL,
    not_integer,  // EZ_VALUE_TYPE_INTEGER,
    not_default,  // EZ_VALUE_TYPE_FLOAT,
    not_default,  // EZ_VALUE_TYPE_STRING,
    not_default,  // EZ_VALUE_TYPE_CAROUSEL,
    not_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_not(ezValue* arg) { return notf[arg->type](m_gc, arg); }

static ezValue* andv_default(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  throw runtime_error("invalid type for AND");
  return NULL;
}

static ezValue* andv_bool(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  bool bret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) bret &= args[i]->to_bool();
  return gc.add(new ezBool(bret));
}

static ezValue* andv_integer(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret &= args[i]->to_integer();
  return gc.add(new ezInteger(iret));
}

ARITHEMATIC_V andv[EZ_VALUE_TYPE_MAX] = {
    andv_default,  // EZ_VALUE_TYPE_NULL = 0,
    andv_default,  // EZ_VALUE_TYPE_CONDITION,
    andv_bool,     // EZ_VALUE_TYPE_BOOL,
    andv_integer,  // EZ_VALUE_TYPE_INTEGER,
    andv_default,  // EZ_VALUE_TYPE_FLOAT,
    andv_default,  // EZ_VALUE_TYPE_STRING,
    andv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    andv_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_and(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = defaultCoercTable[EZ_COERC_OPERATION_AND][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do AND");
  }
  return andv[type](m_gc, args);
}

static ezValue* andd_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for AND");
  return NULL;
}

static ezValue* andd_bool(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezBool(larg->to_bool() & rarg->to_bool()));
}

static ezValue* andd_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezInteger(larg->to_integer() & rarg->to_integer()));
}

ARITHEMATIC_DUO andd[EZ_VALUE_TYPE_MAX] = {
    andd_default,  // EZ_VALUE_TYPE_NULL = 0,
    andd_default,  // EZ_VALUE_TYPE_CONDITION,
    andd_bool,     // EZ_VALUE_TYPE_BOOL,
    andd_integer,  // EZ_VALUE_TYPE_INTEGER,
    andd_default,  // EZ_VALUE_TYPE_FLOAT,
    andd_default,  // EZ_VALUE_TYPE_STRING,
    andd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    andd_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_and(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_AND][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do AND");
  return andd[type](m_gc, larg, rarg);
}

static ezValue* orv_default(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  throw runtime_error("invalid type for OR");
  return NULL;
}

static ezValue* orv_bool(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  bool bret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) bret |= args[i]->to_bool();
  return gc.add(new ezBool(bret));
}

static ezValue* orv_integer(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret |= args[i]->to_integer();
  return gc.add(new ezInteger(iret));
}

ARITHEMATIC_V orv[EZ_VALUE_TYPE_MAX] = {
    orv_default,  // EZ_VALUE_TYPE_NULL = 0,
    orv_default,  // EZ_VALUE_TYPE_CONDITION,
    orv_bool,     // EZ_VALUE_TYPE_BOOL,
    orv_integer,  // EZ_VALUE_TYPE_INTEGER,
    orv_default,  // EZ_VALUE_TYPE_FLOAT,
    orv_default,  // EZ_VALUE_TYPE_STRING,
    orv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    orv_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_or(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = defaultCoercTable[EZ_COERC_OPERATION_OR][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do OR");
  }
  return orv[type](m_gc, args);
}

static ezValue* ord_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for OR");
  return NULL;
}

static ezValue* ord_bool(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezBool(larg->to_bool() | rarg->to_bool()));
}

static ezValue* ord_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezInteger(larg->to_integer() | rarg->to_integer()));
}

ARITHEMATIC_DUO ord[EZ_VALUE_TYPE_MAX] = {
    ord_default,  // EZ_VALUE_TYPE_NULL = 0,
    ord_default,  // EZ_VALUE_TYPE_CONDITION,
    ord_bool,     // EZ_VALUE_TYPE_BOOL,
    ord_integer,  // EZ_VALUE_TYPE_INTEGER,
    ord_default,  // EZ_VALUE_TYPE_FLOAT,
    ord_default,  // EZ_VALUE_TYPE_STRING,
    ord_default,  // EZ_VALUE_TYPE_CAROUSEL,
    ord_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_or(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_OR][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do OR");
  return ord[type](m_gc, larg, rarg);
}

static ezValue* xorv_default(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  throw runtime_error("invalid type for OR");
  return NULL;
}

static ezValue* xorv_bool(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  bool bret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) bret ^= args[i]->to_bool();
  return gc.add(new ezBool(bret));
}

static ezValue* xorv_integer(ezGC<ezValue>& gc, vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret ^= args[i]->to_integer();
  return gc.add(new ezInteger(iret));
}

ARITHEMATIC_V xorv[EZ_VALUE_TYPE_MAX] = {
    xorv_default,  // EZ_VALUE_TYPE_NULL = 0,
    xorv_default,  // EZ_VALUE_TYPE_CONDITION,
    xorv_bool,     // EZ_VALUE_TYPE_BOOL,
    xorv_integer,  // EZ_VALUE_TYPE_INTEGER,
    xorv_default,  // EZ_VALUE_TYPE_FLOAT,
    xorv_default,  // EZ_VALUE_TYPE_STRING,
    xorv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    xorv_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_xor(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = defaultCoercTable[EZ_COERC_OPERATION_XOR][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do XOR");
  }
  return xorv[type](m_gc, args);
}

static ezValue* xord_default(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for XOR");
  return NULL;
}

static ezValue* xord_bool(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezBool(larg->to_bool() ^ rarg->to_bool()));
}

static ezValue* xord_integer(ezGC<ezValue>& gc, ezValue* larg, ezValue* rarg) {
  return gc.add(new ezInteger(larg->to_integer() ^ rarg->to_integer()));
}

ARITHEMATIC_DUO xord[EZ_VALUE_TYPE_MAX] = {
    xord_default,  // EZ_VALUE_TYPE_NULL = 0,
    xord_default,  // EZ_VALUE_TYPE_CONDITION,
    xord_bool,     // EZ_VALUE_TYPE_BOOL,
    xord_integer,  // EZ_VALUE_TYPE_INTEGER,
    xord_default,  // EZ_VALUE_TYPE_FLOAT,
    xord_default,  // EZ_VALUE_TYPE_STRING,
    xord_default,  // EZ_VALUE_TYPE_CAROUSEL,
    xord_default   // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_xor(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      defaultCoercTable[EZ_COERC_OPERATION_XOR][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do XOR");
  return xord[type](m_gc, larg, rarg);
}
