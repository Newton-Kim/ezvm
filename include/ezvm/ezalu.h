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
#pragma once

#include "ezval.h"
#include <vector>

using namespace std;

enum ezCoercOperation {
  EZ_COERC_OPERATION_ADDITION = 0,
  EZ_COERC_OPERATION_SUBTRACTION,
  EZ_COERC_OPERATION_MULTIPLICATION,
  EZ_COERC_OPERATION_DIVISION,
  EZ_COERC_OPERATION_AND,
  EZ_COERC_OPERATION_OR,
  EZ_COERC_OPERATION_XOR,
  EZ_COERC_OPERATION_MAX
};

typedef ezValueType ezCoercTable[EZ_COERC_OPERATION_MAX][EZ_VALUE_TYPE_MAX][EZ_VALUE_TYPE_MAX];
class ezALU {
 private:
  ezCoercTable& m_pCoercTable;

 public:
  ezALU();
  ezALU(ezCoercTable& coercTable);
  ezValue* add(vector<ezValue*>& args);
  ezValue* sub(vector<ezValue*>& args);
  ezValue* mul(vector<ezValue*>& args);
  ezValue* div(vector<ezValue*>& args);
  ezValue* neg(ezValue* arg);
  ezValue* bitwise_and(ezValue* larg, ezValue* rarg);
  ezValue* bitwise_or(ezValue* larg, ezValue* rarg);
  ezValue* bitwise_not(ezValue* arg);
  ezValue* bitwise_xor(ezValue* larg, ezValue* rarg);
};
