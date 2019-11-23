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
#include "ezgc.h"
#include "ezobject.h"
#include "ezstack.h"
#include "eztable.h"
#include <cstddef>
#include <functional>
#include <vector>

using namespace std;

enum ezThreadScheduler { EZ_THREAD_SCHED_REALTIME, EZ_THREAD_SCHED_ROUNDROBIN };

class ezThreadCallback {
public:
  virtual size_t thd(ezAddress &func, vector<ezObject *> &args, ezAddress &ret, ezStackFrame *caller) = 0;
  virtual size_t thd(ezAddress &func, vector<ezObject *> &args, ezStackFrame *caller) = 0;
  virtual bool exist(size_t handle) = 0;
};

/**
 * @brief ezThread is a stack of the instances of ezStackFrame.
 */
class ezThread : public ezStackFrameCallback {
private:
  /**
   * @brief A procedure entry point
   */
  ezAddress m_entry;
  ezThreadScheduler m_scheduler;
  /**
   * @brief A stack of stack frames
   */
  vector<ezStackFrame *> m_stack;
  size_t m_wait;
  ezThreadCallback *m_callback;
  /**
   * @brief fetches a value from an address.
   *
   * @param addr is an address.
   *
   * @return A value.
   */
  ezStackFrame *m_caller;
  ezObject *addr2val(ezAddress addr);

public:
  /**
   * @brief constructs a thread with an entry point, a global memory, and a
   *constant memory.
   *
   * @param entry is a reference to an entry point.
   * @param globals is a reference to a global memory.
   * @param constants is a reference to a constant memory.
   */
  ezThread(ezAddress entry, vector<ezObject *> &args, ezAddress &ret,
           ezThreadCallback *callback,
           ezThreadScheduler sched = EZ_THREAD_SCHED_REALTIME,
           ezStackFrame *caller = NULL);
  ezThread(ezAddress entry, vector<ezObject *> &args, ezThreadCallback *callback,
           ezThreadScheduler sched = EZ_THREAD_SCHED_REALTIME,
           ezStackFrame *caller = NULL);
  /**
   * @brief destroys the thread.
   */
  ~ezThread();
  /**
   * @brief runs an instruction in a stack frame on top of the stack.
   *
   * @return is a state which indicates if there is an instruction to run.
   */
  void run(void);
  void on_mark(void);
  void call(ezStackFrame *sf);
  void end(ezAddress &dest, ezObject * vals);
  size_t thd(ezAddress &func, vector<ezObject *> &args, ezAddress &ret, ezStackFrame *caller);
  size_t thd(ezAddress &func, vector<ezObject *> &args, ezStackFrame *caller);
  void wait(size_t handle);
  bool empty(void) { return m_stack.empty(); }
  void dump(ezFile &sink);
};
