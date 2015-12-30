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
        // EZ_COERC_OPERATION_MODULATION,
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

typedef ezValue* (*ARITHEMATIC_V)(vector<ezValue*>& args);
typedef ezValue* (*ARITHEMATIC_DUO)(ezValue* larg, ezValue* rarg);
typedef ezValue* (*ARITHEMATIC_UNI)(ezValue* arg);

static ezValue* addv_default(vector<ezValue*>& args) {
  throw runtime_error("invalid type for addition");
  return NULL;
}

static ezValue* addv_integer(vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret += args[i]->to_integer();
  return new ezInteger(iret);
}

static ezValue* addv_string(vector<ezValue*>& args) {
  string sret = args[0]->to_string();
  for (size_t i = 1; i < args.size(); i++) sret += args[i]->to_string();
  return new ezString(sret);
}

ARITHEMATIC_V addv[EZ_VALUE_TYPE_MAX] = {
    addv_default,  // EZ_VALUE_TYPE_NULL = 0,
    addv_default,  // EZ_VALUE_TYPE_CONDITION,
    addv_default,  // EZ_VALUE_TYPE_BOOL,
    addv_integer,  // EZ_VALUE_TYPE_INTEGER,
    addv_string,  // EZ_VALUE_TYPE_STRING,
    addv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    addv_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::add(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_ADDITION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do addition");
  }
  return addv[type](args);
}

static ezValue* addd_default(ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for addition");
  return NULL;
}

static ezValue* addd_integer(ezValue* larg, ezValue* rarg) {
  return new ezInteger(larg->to_integer() + rarg->to_integer());
}

static ezValue* addd_string(ezValue* larg, ezValue* rarg) {
  return new ezString(larg->to_string() + rarg->to_string());
}

ARITHEMATIC_DUO addd[EZ_VALUE_TYPE_MAX] = {
    addd_default,  // EZ_VALUE_TYPE_NULL = 0,
    addd_default,  // EZ_VALUE_TYPE_CONDITION,
    addd_default,  // EZ_VALUE_TYPE_BOOL,
    addd_integer,  // EZ_VALUE_TYPE_INTEGER,
    addd_string,  // EZ_VALUE_TYPE_STRING,
    addd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    addd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::add(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_ADDITION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do addition");
  return addd[type](larg, rarg);
}

static ezValue* subv_default(vector<ezValue*>& args) {
  throw runtime_error("invalid type for subtraction");
  return NULL;
}

static ezValue* subv_integer(vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret -= args[i]->to_integer();
  return new ezInteger(iret);
}

ARITHEMATIC_V subv[EZ_VALUE_TYPE_MAX] = {
    subv_default,  // EZ_VALUE_TYPE_NULL = 0,
    subv_default,  // EZ_VALUE_TYPE_CONDITION,
    subv_default,  // EZ_VALUE_TYPE_BOOL,
    subv_integer,  // EZ_VALUE_TYPE_INTEGER,
    subv_default,  // EZ_VALUE_TYPE_STRING,
    subv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    subv_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::sub(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_SUBTRACTION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do subtraction");
  }
  return subv[type](args);
}

static ezValue* subd_default(ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for subtraction");
  return NULL;
}

static ezValue* subd_integer(ezValue* larg, ezValue* rarg) {
  return new ezInteger(larg->to_integer() - rarg->to_integer());
}

ARITHEMATIC_DUO subd[EZ_VALUE_TYPE_MAX] = {
    subd_default,  // EZ_VALUE_TYPE_NULL = 0,
    subd_default,  // EZ_VALUE_TYPE_CONDITION,
    subd_default,  // EZ_VALUE_TYPE_BOOL,
    subd_integer,  // EZ_VALUE_TYPE_INTEGER,
    subd_default,  // EZ_VALUE_TYPE_STRING,
    subd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    subd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::sub(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_SUBTRACTION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do subtraction");
  return subd[type](larg, rarg);
}

static ezValue* modv_default(vector<ezValue*>& args) {
  throw runtime_error("invalid type for modulation");
  return NULL;
}

static ezValue* modv_integer(vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret %= args[i]->to_integer();
  return new ezInteger(iret);
}

ARITHEMATIC_V modv[EZ_VALUE_TYPE_MAX] = {
    modv_default,  // EZ_VALUE_TYPE_NULL = 0,
    modv_default,  // EZ_VALUE_TYPE_CONDITION,
    modv_default,  // EZ_VALUE_TYPE_BOOL,
    modv_integer,  // EZ_VALUE_TYPE_INTEGER,
    modv_default,  // EZ_VALUE_TYPE_STRING,
    modv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    modv_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::mod(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do modulation");
  }
  return modv[type](args);
}

static ezValue* modd_default(ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for modulation");
  return NULL;
}

static ezValue* modd_integer(ezValue* larg, ezValue* rarg) {
  return new ezInteger(larg->to_integer() % rarg->to_integer());
}

ARITHEMATIC_DUO modd[EZ_VALUE_TYPE_MAX] = {
    modd_default,  // EZ_VALUE_TYPE_NULL = 0,
    modd_default,  // EZ_VALUE_TYPE_CONDITION,
    modd_default,  // EZ_VALUE_TYPE_BOOL,
    modd_integer,  // EZ_VALUE_TYPE_INTEGER,
    modd_default,  // EZ_VALUE_TYPE_STRING,
    modd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    modd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::mod(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do modulation");
  return modd[type](larg, rarg);
}

static ezValue* mulv_default(vector<ezValue*>& args) {
  throw runtime_error("invalid type for multiplication");
  return NULL;
}

static ezValue* mulv_integer(vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret *= args[i]->to_integer();
  return new ezInteger(iret);
}

ARITHEMATIC_V mulv[EZ_VALUE_TYPE_MAX] = {
    mulv_default,  // EZ_VALUE_TYPE_NULL = 0,
    mulv_default,  // EZ_VALUE_TYPE_CONDITION,
    mulv_default,  // EZ_VALUE_TYPE_BOOL,
    mulv_integer,  // EZ_VALUE_TYPE_INTEGER,
    mulv_default,  // EZ_VALUE_TYPE_STRING,
    mulv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    mulv_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::mul(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do multiplication");
  }
  return mulv[type](args);
}

static ezValue* muld_default(ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for multiplication");
  return NULL;
}

static ezValue* muld_integer(ezValue* larg, ezValue* rarg) {
  return new ezInteger(larg->to_integer() * rarg->to_integer());
}

ARITHEMATIC_DUO muld[EZ_VALUE_TYPE_MAX] = {
    muld_default,  // EZ_VALUE_TYPE_NULL = 0,
    muld_default,  // EZ_VALUE_TYPE_CONDITION,
    muld_default,  // EZ_VALUE_TYPE_BOOL,
    muld_integer,  // EZ_VALUE_TYPE_INTEGER,
    muld_default,  // EZ_VALUE_TYPE_STRING,
    muld_default,  // EZ_VALUE_TYPE_CAROUSEL,
    muld_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::mul(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_MULTIPLICATION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do multiplication");
  return muld[type](larg, rarg);
}

static ezValue* divv_default(vector<ezValue*>& args) {
  throw runtime_error("invalid type for division");
  return NULL;
}

static ezValue* divv_integer(vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret /= args[i]->to_integer();
  return new ezInteger(iret);
}

ARITHEMATIC_V divv[EZ_VALUE_TYPE_MAX] = {
    divv_default,  // EZ_VALUE_TYPE_NULL = 0,
    divv_default,  // EZ_VALUE_TYPE_CONDITION,
    divv_default,  // EZ_VALUE_TYPE_BOOL,
    divv_integer,  // EZ_VALUE_TYPE_INTEGER,
    divv_default,  // EZ_VALUE_TYPE_STRING,
    divv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    divv_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::div(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_DIVISION][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do division");
  }
  return divv[type](args);
}

static ezValue* divd_default(ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for division");
  return NULL;
}

static ezValue* divd_integer(ezValue* larg, ezValue* rarg) {
  return new ezInteger(larg->to_integer() / rarg->to_integer());
}

ARITHEMATIC_DUO divd[EZ_VALUE_TYPE_MAX] = {
    divd_default,  // EZ_VALUE_TYPE_NULL = 0,
    divd_default,  // EZ_VALUE_TYPE_CONDITION,
    divd_default,  // EZ_VALUE_TYPE_BOOL,
    divd_integer,  // EZ_VALUE_TYPE_INTEGER,
    divd_default,  // EZ_VALUE_TYPE_STRING,
    divd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    divd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::div(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_DIVISION][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do division");
  return divd[type](larg, rarg);
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

static ezValue* not_default(ezValue* arg) {
  throw runtime_error("invalid type for NOT");
  return NULL;
}

static ezValue* not_bool(ezValue* arg) {
  return new ezBool(!arg->to_bool());
}

static ezValue* not_integer(ezValue* arg) {
  return new ezInteger(~arg->to_integer());
}

ARITHEMATIC_UNI notf[EZ_VALUE_TYPE_MAX] = {
    not_default,  // EZ_VALUE_TYPE_NULL = 0,
    not_default,  // EZ_VALUE_TYPE_CONDITION,
    not_bool,  // EZ_VALUE_TYPE_BOOL,
    not_integer,  // EZ_VALUE_TYPE_INTEGER,
    not_default,  // EZ_VALUE_TYPE_STRING,
    not_default,  // EZ_VALUE_TYPE_CAROUSEL,
    not_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};


ezValue* ezALU::bitwise_not(ezValue* arg) {
  return notf[arg->type](arg);
}

static ezValue* andv_default(vector<ezValue*>& args) {
  throw runtime_error("invalid type for AND");
  return NULL;
}

static ezValue* andv_bool(vector<ezValue*>& args) {
  bool bret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) bret &= args[i]->to_bool();
  return new ezBool(bret);
}

static ezValue* andv_integer(vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret &= args[i]->to_integer();
  return new ezInteger(iret);
}

ARITHEMATIC_V andv[EZ_VALUE_TYPE_MAX] = {
    andv_default,  // EZ_VALUE_TYPE_NULL = 0,
    andv_default,  // EZ_VALUE_TYPE_CONDITION,
    andv_bool,  // EZ_VALUE_TYPE_BOOL,
    andv_integer,  // EZ_VALUE_TYPE_INTEGER,
    andv_default,  // EZ_VALUE_TYPE_STRING,
    andv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    andv_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_and(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_AND][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do AND");
  }
  return andv[type](args);
}

static ezValue* andd_default(ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for AND");
  return NULL;
}

static ezValue* andd_bool(ezValue* larg, ezValue* rarg) {
  return new ezBool(larg->to_bool() & rarg->to_bool());
}

static ezValue* andd_integer(ezValue* larg, ezValue* rarg) {
  return new ezInteger(larg->to_integer() & rarg->to_integer());
}

ARITHEMATIC_DUO andd[EZ_VALUE_TYPE_MAX] = {
    andd_default,  // EZ_VALUE_TYPE_NULL = 0,
    andd_default,  // EZ_VALUE_TYPE_CONDITION,
    andd_bool,  // EZ_VALUE_TYPE_BOOL,
    andd_integer,  // EZ_VALUE_TYPE_INTEGER,
    andd_default,  // EZ_VALUE_TYPE_STRING,
    andd_default,  // EZ_VALUE_TYPE_CAROUSEL,
    andd_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_and(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_AND][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do AND");
  return andd[type](larg, rarg);
}

static ezValue* orv_default(vector<ezValue*>& args) {
  throw runtime_error("invalid type for OR");
  return NULL;
}

static ezValue* orv_bool(vector<ezValue*>& args) {
  bool bret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) bret |= args[i]->to_bool();
  return new ezBool(bret);
}

static ezValue* orv_integer(vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret |= args[i]->to_integer();
  return new ezInteger(iret);
}

ARITHEMATIC_V orv[EZ_VALUE_TYPE_MAX] = {
    orv_default,  // EZ_VALUE_TYPE_NULL = 0,
    orv_default,  // EZ_VALUE_TYPE_CONDITION,
    orv_bool,  // EZ_VALUE_TYPE_BOOL,
    orv_integer,  // EZ_VALUE_TYPE_INTEGER,
    orv_default,  // EZ_VALUE_TYPE_STRING,
    orv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    orv_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_or(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_OR][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do OR");
  }
  return orv[type](args);
}

static ezValue* ord_default(ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for OR");
  return NULL;
}

static ezValue* ord_bool(ezValue* larg, ezValue* rarg) {
  return new ezBool(larg->to_bool() | rarg->to_bool());
}

static ezValue* ord_integer(ezValue* larg, ezValue* rarg) {
  return new ezInteger(larg->to_integer() | rarg->to_integer());
}

ARITHEMATIC_DUO ord[EZ_VALUE_TYPE_MAX] = {
    ord_default,  // EZ_VALUE_TYPE_NULL = 0,
    ord_default,  // EZ_VALUE_TYPE_CONDITION,
    ord_bool,  // EZ_VALUE_TYPE_BOOL,
    ord_integer,  // EZ_VALUE_TYPE_INTEGER,
    ord_default,  // EZ_VALUE_TYPE_STRING,
    ord_default,  // EZ_VALUE_TYPE_CAROUSEL,
    ord_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_or(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_OR][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do OR");
  return ord[type](larg, rarg);
}

static ezValue* xorv_default(vector<ezValue*>& args) {
  throw runtime_error("invalid type for OR");
  return NULL;
}

static ezValue* xorv_bool(vector<ezValue*>& args) {
  bool bret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) bret ^= args[i]->to_bool();
  return new ezBool(bret);
}

static ezValue* xorv_integer(vector<ezValue*>& args) {
  int iret = args[0]->to_integer();
  for (size_t i = 1; i < args.size(); i++) iret ^= args[i]->to_integer();
  return new ezInteger(iret);
}

ARITHEMATIC_V xorv[EZ_VALUE_TYPE_MAX] = {
    xorv_default,  // EZ_VALUE_TYPE_NULL = 0,
    xorv_default,  // EZ_VALUE_TYPE_CONDITION,
    xorv_bool,  // EZ_VALUE_TYPE_BOOL,
    xorv_integer,  // EZ_VALUE_TYPE_INTEGER,
    xorv_default,  // EZ_VALUE_TYPE_STRING,
    xorv_default,  // EZ_VALUE_TYPE_CAROUSEL,
    xorv_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_xor(vector<ezValue*>& args) {
  ezValueType type = args[0]->type;
  for (size_t i = 1; i < args.size(); i++) {
    type = m_pCoercTable[EZ_COERC_OPERATION_XOR][type][args[i]->type];
    if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do XOR");
  }
  return xorv[type](args);
}

static ezValue* xord_default(ezValue* larg, ezValue* rarg) {
  throw runtime_error("invalid type for XOR");
  return NULL;
}

static ezValue* xord_bool(ezValue* larg, ezValue* rarg) {
  return new ezBool(larg->to_bool() ^ rarg->to_bool());
}

static ezValue* xord_integer(ezValue* larg, ezValue* rarg) {
  return new ezInteger(larg->to_integer() ^ rarg->to_integer());
}

ARITHEMATIC_DUO xord[EZ_VALUE_TYPE_MAX] = {
    xord_default,  // EZ_VALUE_TYPE_NULL = 0,
    xord_default,  // EZ_VALUE_TYPE_CONDITION,
    xord_bool,  // EZ_VALUE_TYPE_BOOL,
    xord_integer,  // EZ_VALUE_TYPE_INTEGER,
    xord_default,  // EZ_VALUE_TYPE_STRING,
    xord_default,  // EZ_VALUE_TYPE_CAROUSEL,
    xord_default  // EZ_VALUE_TYPE_NATIVE_CAROUSEL
};

ezValue* ezALU::bitwise_xor(ezValue* larg, ezValue* rarg) {
  ezValueType type =
      m_pCoercTable[EZ_COERC_OPERATION_XOR][larg->type][rarg->type];
  if (type == EZ_VALUE_TYPE_MAX) throw runtime_error("unable to do XOR");
  return xord[type](larg, rarg);
}
