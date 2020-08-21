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
#include "ezfunc.h"
#include "ezsegment.h"
#include "ezval.h"
#include <functional>
#include <stack>
#include <vector>

using namespace std;

class ezStackFrameCallback {
public:
  virtual void call(ezStackFrame *sf) = 0;
  virtual void end(ezAddress &dests, ezObject *val) = 0;
  virtual size_t thd(ezAddress &func, vector<ezObject *> &args, ezAddress &ret,
                     ezStackFrame *caller) = 0;
  virtual size_t thd(ezAddress &func, vector<ezObject *> &args,
                     ezStackFrame *caller) = 0;
  virtual void wait(size_t handle) = 0;
};

/**
 * @brief contains data for representing a procedure in action.
 */
class ezStackFrame : public ezGCObject {
private:
  /**
   * @brief is a program counter.
   */
  size_t m_pc;
  /**
   * @brief is a pointer to a carousel.
   */
  ezFunction *m_carousel;
  /**
   * @brief is a local segment.
   */
  ezMemSegment *m_local;
  /**
   * @brief is a temporary segment.
   */
  vector<ezObject *> m_temporary;
  /**
   * @brief is a scope segment.
   */
  ezMemSegment *m_scope;
  /**
   * @brief is a collection of return values.
   */
  ezObject *m_return;
  /**
   * @brief is a collection of addresses where the result values place.
   */
  ezAddress m_return_dest;
  ezStackFrameCallback *m_callback;
  vector<vector<ezObject *> *> m_memory;
  ezALU *m_alu;
  void initialise(ezStackFrame *caller, vector<ezObject *> &args);

public:
  void addr2val(vector<ezObject *> &vals, vector<ezAddress> &addr);
  /**
   * @brief fetches a value from an address.
   *
   * @param addr is an address.
   *
   * @return A value.
   */
  ezObject *addr2val(ezAddress addr);
  /**
   * @brief places a value at an address.
   *
   * @param addr is an address to place v.
   * @param v is a value.
   */
  void val2addr(ezAddress addr, ezObject *v);
  /**
   * @brief places values at respective addresses.
   *
   * @param addr is an array of addresses.
   * @param v is an array of values.
   */
  void val2addr(vector<ezAddress> &addr, vector<ezObject *> &vals);

private:
  /**
   * @brief invokes a native carousel.
   *
   * @param func is a pointer to a native carousel.
   * @param args is a number of arguments which follows the func.
   * @param ret is a number of return addresses which follows the arguments.
   */
  void call(ezUserDefinedFunction *func, vector<ezObject *> &args,
            ezAddress &ret);
  /**
   * @brief invokes a native carousel.
   *
   * @param func is a pointer to a native carousel.
   * @param args is a number of arguments which follows the func.
   */
  void call(ezUserDefinedFunction *func, vector<ezObject *> &args);
  /**
   * @brief invokes a carousel.
   *
   * @param func is a pointer to a native carousel.
   * @param args is a number of arguments which follows the func.
   * @param ret is a number of return addresses which follows the arguments.
   */
  void call(ezFunction *func, vector<ezObject *> &args, ezAddress &ret);
  /**
   * @brief invokes a carousel.
   *
   * @param func is a pointer to a native carousel.
   * @param args is a number of arguments which follows the func.
   */
  void call(ezFunction *func, vector<ezObject *> &args);
  void conditional_bra(ezAddress &cond, size_t index,
                       function<bool(ezCondition *)> func);
  /*
  void test_equality(ezAddress &rst, ezAddress &lsrc, ezAddress &rsrc,
                     function<ezValue *(ezCondition *)> func);
  void test_equality(ezAddress &rst, ezAddress &cond, ezAddress &lsrc,
                     ezAddress &rsrc, function<ezValue *(ezCondition *)> func);
		     */

public:
  /*
    void teq(ezAddress &dest, ezAddress &src1, ezAddress &src2);
    void teq(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress
    &src2); void tge(ezAddress &dest, ezAddress &src1, ezAddress &src2); void
    tge(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
    void tlt(ezAddress &dest, ezAddress &src1, ezAddress &src2);
    void tlt(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress
    &src2); void tne(ezAddress &dest, ezAddress &src1, ezAddress &src2); void
    tne(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  */
  void beq(ezAddress &cond, size_t index);
  void bge(ezAddress &cond, size_t index);
  void blt(ezAddress &cond, size_t index);
  void bne(ezAddress &cond, size_t index);
  void bra(size_t index);
  void ret(vector<ezAddress> &srcs);
  void call(ezAddress &func, vector<ezAddress> &args, ezAddress &ret);
  void call(ezAddress &func, vector<ezAddress> &args);
  void thd(ezAddress &func, vector<ezAddress> &args, ezAddress &ret,
           ezAddress &handle);
  void thd(ezAddress &func, vector<ezAddress> &args, ezAddress &handle);
  void wait(ezAddress &handle);
  void fgc(void);

private:
  void lsr(uint8_t ndests, uint8_t nsrcs, uint8_t offsets);

public:
  /**
   * @brief constructs the instance with a carousel.
   *
   * @param crsl is a pointer to a carousel.
   */
  ezStackFrame(ezFunction *crsl, vector<ezObject *> &args, ezAddress &ret,
               ezStackFrame *caller, ezStackFrameCallback *callback);
  ezStackFrame(ezFunction *crsl, vector<ezObject *> &args, ezStackFrame *caller,
               ezStackFrameCallback *callback);
  /**
   * @brief is a destructor.
   */
  ~ezStackFrame();
  void step(void); // the arguments are temporary ones.
  void update(ezAddress &dests, ezObject *vals);
  void on_mark(void);
  void dump(ezFile &sink);
};
