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
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,    EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_STRING, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_SUBTRACTION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_MULTIPLICATION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_DIVISION,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_AND,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_OR,
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        },
        // EZ_COERC_OPERATION_XOR
        {
            // EZ_VALUE_TYPE_NULL EZ_VALUE_TYPE_CONDITION, EZ_VALUE_TYPE_BOOL,
            // EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_STRING,
            // EZ_VALUE_TYPE_CAROUSEL, EZ_VALUE_TYPE_NATIVE_CAROUSEL
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_NULL = 0,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CONDITION,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_BOOL,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_BOOL,
            {EZ_VALUE_TYPE_MAX,     EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_INTEGER, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_INTEGER,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_STRING,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX},  // EZ_VALUE_TYPE_CAROUSEL,
            {EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX, EZ_VALUE_TYPE_MAX,
             EZ_VALUE_TYPE_MAX}  // EZ_VALUE_TYPE_NATIVE_CAROUSEL,
        }};

ezALU::ezALU() : m_pCoercTable(defaultCoercTable) {}

ezALU::ezALU(ezCoercTable& coercTable) : m_pCoercTable(coercTable) {}

ezValue* ezALU::add(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_ADDITION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do addition");
  }
  int iret = 0;
  string sret;
  ezValue* ret = NULL;
  switch (type) {
    case EZ_VALUE_TYPE_INTEGER:
      iret = args[0]->to_integer();
      for (size_t i = 1; i < args.size(); i++) iret += args[i]->to_integer();
      ret = new ezInteger(iret);
      break;
    case EZ_VALUE_TYPE_STRING:
      sret = args[0]->to_string();
      for (size_t i = 1; i < args.size(); i++) sret += args[i]->to_string();
      ret = new ezString(sret);
      break;
  }
  return ret;
}

ezValue* ezALU::sub(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_SUBTRACTION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do addition");
  }
  int iret = 0;
  string sret;
  ezValue* ret = NULL;
  switch (type) {
    case EZ_VALUE_TYPE_INTEGER:
      iret = args[0]->to_integer();
      for (size_t i = 1; i < args.size(); i++) iret -= args[i]->to_integer();
      ret = new ezInteger(iret);
      break;
  }
  return ret;
}

ezValue* ezALU::mul(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type =
        m_pCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do addition");
  }
  int iret = 0;
  string sret;
  ezValue* ret = NULL;
  switch (type) {
    case EZ_VALUE_TYPE_INTEGER:
      iret = args[0]->to_integer();
      for (size_t i = 1; i < args.size(); i++) iret *= args[i]->to_integer();
      ret = new ezInteger(iret);
      break;
  }
  return ret;
}

ezValue* ezALU::div(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_DIVISION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do addition");
  }
  int iret = 0;
  string sret;
  ezValue* ret = NULL;
  switch (type) {
    case EZ_VALUE_TYPE_INTEGER:
      iret = args[0]->to_integer();
      for (size_t i = 1; i < args.size(); i++) iret /= args[i]->to_integer();
      ret = new ezInteger(iret);
      break;
  }
  return ret;
}

ezValue* ezALU::bitwise_and(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_AND][larg->type][rarg->type];
  ezValue* ret = NULL;
  switch (type) {
    case EZ_VALUE_TYPE_BOOL:
      ret = new ezBool(larg->to_bool() & rarg->to_bool());
      break;
    case EZ_VALUE_TYPE_INTEGER:
      ret = new ezInteger(larg->to_integer() & rarg->to_integer());
      break;
    default:
      throw runtime_error("unable to do AND operation");
  }
  return ret;
}

ezValue* ezALU::bitwise_or(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_OR][larg->type][rarg->type];
  ezValue* ret = NULL;
  switch (type) {
    case EZ_VALUE_TYPE_BOOL:
      ret = new ezBool(larg->to_bool() | rarg->to_bool());
      break;
    case EZ_VALUE_TYPE_INTEGER:
      ret = new ezInteger(larg->to_integer() | rarg->to_integer());
      break;
    default:
      throw runtime_error("unable to do AND operation");
  }
  return ret;
}

ezValue* ezALU::bitwise_not(ezValue* arg) {
  ezValue* ret = NULL;
  switch (arg->type) {
    case EZ_VALUE_TYPE_BOOL:
      ret = new ezBool(!arg->to_bool());
      break;
    case EZ_VALUE_TYPE_INTEGER:
      ret = new ezInteger(~arg->to_integer());
      break;
  }
  return ret;
}

ezValue* ezALU::bitwise_xor(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_XOR][larg->type][rarg->type];
  ezValue* ret = NULL;
  switch (type) {
    case EZ_VALUE_TYPE_BOOL:
      ret = new ezBool(larg->to_bool() ^ rarg->to_bool());
      break;
    case EZ_VALUE_TYPE_INTEGER:
      ret = new ezInteger(larg->to_integer() ^ rarg->to_integer());
      break;
    default:
      throw runtime_error("unable to do AND operation");
  }
  return ret;
}
