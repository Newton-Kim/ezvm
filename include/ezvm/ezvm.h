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
#include "asm/ezasm.h"
#include "ezlog.h"
#include "ezobject.h"
#include "eztable.h"
#include "ezthread.h"

#include <list>
#include <string>

using namespace std;

/**
 * @brief ezVM is the VM class
 */
class ezVM : public ezGCObject, ezThreadCallback {
private:
  ezAddress m_entry;
  ezASM *m_pasm;
  // TODO:user defined dump should be pluggable.
  list<ezThread *> m_threads;

protected:
  void on_mark(void);

public:
  ezVM();
  ~ezVM();
  void run(void);
  ezASM &assembler(void);
  size_t thd(ezAddress &func, vector<ezObject *> &args, ezAddress &ret,
             ezStackFrame *caller);
  size_t thd(ezAddress &func, vector<ezObject *> &args, ezStackFrame *caller);
  bool exist(size_t handle);
  void dump(string path);
};
