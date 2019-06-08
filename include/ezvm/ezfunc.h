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
#include "ezaddr.h"
#include "ezinstruction.h"
#include "eztable.h"
#include "ezobject.h"

class ezFunction : public ezObject {
private:
  ezTable<string, ezObject *> *m_scope;
  ezTable<string, ezObject *> *m_local;

public:
  uint8_t nargs;
  size_t nmems;
  ezFunction(ezTable<string, ezObject *> *local,
             ezTable<string, ezObject *> *scope);
  ~ezFunction();
  vector<ezInstruction *> instruction;
  ezTable<string, size_t> jmptbl;
  void on_mark(void);
  vector<ezObject *> *local_memory(void);
  vector<ezObject *> *scope_memory(void);
  bool is_local_scoped(void) { return (m_local) ? true : false; }
};

class ezUserDefinedFunction : public ezObject{
public:
  ezUserDefinedFunction();
  virtual ~ezUserDefinedFunction() {}

  virtual void run(vector<ezObject *> &args, vector<ezObject *> &rets) = 0;
};

class ezCondition : public ezObject {
public:
  const bool zero;
  const bool negative;
  const bool overflow;
  const bool carry;
  ezCondition(const bool zr, const bool neg, const bool ovf, const bool cry);
};

