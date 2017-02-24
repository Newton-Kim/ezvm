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

#include "ezgc.h"
#include "ezval.h"
#include <vector>

using namespace std;

enum ezCoercOperation {
  EZ_COERC_OPERATION_ADDITION = 0,
  EZ_COERC_OPERATION_COMPARISON,
  EZ_COERC_OPERATION_SUBTRACTION,
  EZ_COERC_OPERATION_MULTIPLICATION,
  EZ_COERC_OPERATION_DIVISION,
  EZ_COERC_OPERATION_MODULATION,
  EZ_COERC_OPERATION_AND,
  EZ_COERC_OPERATION_OR,
  EZ_COERC_OPERATION_XOR,
  EZ_COERC_OPERATION_MAX
};

class ezUsrALU {
public:
  virtual ~ezUsrALU() {};
  virtual ezValue *add(ezValue *larg, ezValue *rarg) = 0;
  virtual ezValue *cmp(ezValue *larg, ezValue *rarg) = 0;
  virtual ezValue *sub(ezValue *larg, ezValue *rarg) = 0;
  virtual ezValue *mod(ezValue *larg, ezValue *rarg) = 0;
  virtual ezValue *mul(ezValue *larg, ezValue *rarg) = 0;
  virtual ezValue *div(ezValue *larg, ezValue *rarg) = 0;
  virtual ezValue *neg(ezValue *arg) = 0;
  virtual ezValue *bitwise_not(ezValue *arg) = 0;
  virtual ezValue *bitwise_and(ezValue *larg, ezValue *rarg) = 0;
  virtual ezValue *bitwise_or(ezValue *larg, ezValue *rarg) = 0;
  virtual ezValue *bitwise_xor(ezValue *larg, ezValue *rarg) = 0;
};

class ezALU {
private:
  ezGC &m_gc;
  ezUsrALU *m_usr_alu;

public:
  ezALU(ezGC &gc, ezUsrALU *usr_alu);
  ezValue *add(ezValue *larg, ezValue *rarg);
  ezValue *cmp(ezValue *larg, ezValue *rarg);
  ezValue *sub(ezValue *larg, ezValue *rarg);
  ezValue *mod(ezValue *larg, ezValue *rarg);
  ezValue *mul(ezValue *larg, ezValue *rarg);
  ezValue *div(ezValue *larg, ezValue *rarg);
  ezValue *neg(ezValue *arg);
  ezValue *bitwise_not(ezValue *arg);
  ezValue *bitwise_and(ezValue *larg, ezValue *rarg);
  ezValue *bitwise_or(ezValue *larg, ezValue *rarg);
  ezValue *bitwise_xor(ezValue *larg, ezValue *rarg);
};
