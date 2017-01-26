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
#include "ezalu.h"
#include "ezgc.h"
#include "ezstack.h"
#include "eztable.h"
#include "ezval.h"
#include <cstddef>
#include <functional>
#include <vector>

using namespace std;

/**
* @brief ezThread is a stack of the instances of ezStackFrame.
*/
class ezThread {
private:
  /**
  * @brief A procedure entry point
  */
  ezAddress m_entry;
  /**
  * @brief A reference to a constant segment
  */
  vector<ezValue *> &m_constants;
  /**
  * @brief An arguments to a new stack frame
  */
  vector<ezValue *> m_args;
  /**
  * @brief A return values from the stack frame on top
  */
  vector<ezValue *> m_rets;
  /**
  * @brief A reference to a global segment
  */
  ezTable<string, ezValue *> &m_globals;
  /**
  * @brief A stack of stack frames
  */
  vector<ezStackFrame *> m_stack;
  /**
  * @brief An arithematic logic unit
  */
  ezALU &m_alu;
  /**
  * @brief A garbage collector
  */
  ezGC &m_gc;
  /**
  * @brief fetches a value from an address.
  *
  * @param addr is an address.
  *
  * @return A value.
  */
  ezValue *addr2val(ezAddress addr);

public:
  /**
  * @brief constructs a thread with an entry point, a global memory, and a
  *constant memory.
  *
    * @param entry is a reference to an entry point.
    * @param globals is a reference to a global memory.
    * @param constants is a reference to a constant memory.
  */
  ezThread(ezAddress entry, ezTable<string, ezValue *> &globals,
           vector<ezValue *> &constants, ezALU &alu, ezGC &gc);
  /**
  * @brief destroys the thread.
  */
  ~ezThread();
  /**
  * @brief runs an instruction in a stack frame on top of the stack.
  *
  * @return is a state which indicates if there is an instruction to run.
  */
  ezStepState step(void);
  void on_mark(void);
};
