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
#include "ezval.h"
#include <functional>
#include <stack>
#include <vector>

using namespace std;

class ezStackFrameCallback {
public:
  virtual void call(ezStackFrame* sf) = 0;
  virtual void end(void) = 0;
};

/**
* @brief contains data for representing a procedure in action.
*/
class ezStackFrame {
private:
  /**
  * @brief is a program counter.
  */
  size_t m_pc;
  /**
  * @brief is a pointer to a carousel.
  */
  ezCarousel *m_carousel;
  /**
  * @brief is a local segment.
  */
  vector<ezValue *> *m_local;
  /**
  * @brief is a scope segment.
  */
  vector<ezValue *> *m_scope;
  /**
  * @brief is a collection of return values.
  */
  vector<ezValue *> m_returns;
  /**
  * @brief is a collection of addresses where the result values place.
  */
  vector<ezAddress> m_return_dest;
  /**
  * @brief A reference to a constant segment
  */
  vector<ezValue *> &m_constants;
  /**
  * @brief A reference to a global segment
  */
  ezTable<string, ezValue *> &m_globals;
  /**
  * @brief An arithematic logic unit
  */
  ezALU &m_alu;
  /**
  * @brief A garbage collector
  */
  ezGC &m_gc;
  ezStackFrameCallback *m_callback;
  vector<vector<ezValue *> *> m_memory;
  /**
  * @brief fetches a value from an address.
  *
  * @param addr is an address.
  *
  * @return A value.
  */
  ezValue *addr2val(ezAddress addr);
  /**
  * @brief places a value at an address.
  *
  * @param addr is an address to place v.
  * @param v is a value.
  */
  void val2addr(ezAddress addr, ezValue *v);
  /**
  * @brief places values at respective addresses.
  *
  * @param addr is an array of addresses.
  * @param v is an array of values.
  */
  void val2addr(vector<ezAddress> &addr, vector<ezValue *> &vals);
  /**
  * @brief invokes a native carousel.
  *
  * @param func is a pointer to a native carousel.
  * @param nargs is a number of arguments which follows the func.
  * @param nrets is a number of return addresses which follows the arguments.
  */
  void call(ezNativeCarousel *func, vector<ezAddress> &args,
                   vector<ezAddress> &rets);
  /**
  * @brief invokes a carousel.
  *
  * @param func is a pointer to a native carousel.
  * @param nargs is a number of arguments which follows the func.
  * @param nrets is a number of return addresses which follows the arguments.
  */
  void call(ezCarousel *func, vector<ezAddress> &args,
                   vector<ezAddress> &rets);
  void binary_operation(ezAddress &dest, ezAddress &src1, ezAddress &src2,
                        function<ezValue *(ezValue *, ezValue *)> binary_func);
  void binary_operation(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                        ezAddress &src2,
                        function<ezValue *(ezValue *, ezValue *)> binary_func);
  void conditional_bra(ezAddress &cond, size_t index,
                       function<bool(ezCondition *)> func);
  void shift_operation(ezAddress &dest, ezAddress &src, ezAddress &offset,
                       function<ezValue *(ezValue *, ezValue *)> func);
  void shift_operation(ezAddress &dest, ezAddress &cond, ezAddress &src,
                       ezAddress &offset,
                       function<ezValue *(ezValue *, ezValue *)> func);
  void unary_operation(ezAddress &dest, ezAddress &src,
                       function<ezValue *(ezValue *)> binary_func);
  void unary_operation(ezAddress &dest, ezAddress &cond, ezAddress &src,
                       function<ezValue *(ezValue *)> binary_func);
  ezValue *test_equality(ezAddress &rst, ezAddress &lsrc, ezAddress &rsrc,
                         function<ezValue *(ezCondition *)> func);

public:
  void add(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void add(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void cmp(ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void div(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void div(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void mod(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void mod(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void mul(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void mul(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void sub(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void sub(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void bitwise_and(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void bitwise_and(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                   ezAddress &src2);
  void bitwise_or(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void bitwise_or(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                  ezAddress &src2);
  void teq(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void teq(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void tge(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void tge(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void tlt(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void tlt(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void tne(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void tne(ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2);
  void bitwise_xor(ezAddress &dest, ezAddress &src1, ezAddress &src2);
  void bitwise_xor(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                   ezAddress &src2);
  void lsl(ezAddress &dest, ezAddress &src, ezAddress &offset);
  void lsl(ezAddress &dest, ezAddress &cond, ezAddress &src, ezAddress &offset);
  void lsr(ezAddress &dest, ezAddress &src, ezAddress &offset);
  void lsr(ezAddress &dest, ezAddress &cond, ezAddress &src, ezAddress &offset);
  void beq(ezAddress &cond, size_t index);
  void bge(ezAddress &cond, size_t index);
  void blt(ezAddress &cond, size_t index);
  void bne(ezAddress &cond, size_t index);
  void bra(size_t index);
  void neg(ezAddress &dest, ezAddress &src);
  void neg(ezAddress &dest, ezAddress &cond, ezAddress &src);
  void bitwise_not(ezAddress &dest, ezAddress &src);
  void bitwise_not(ezAddress &dest, ezAddress &cond, ezAddress &src);
  void mv(vector<ezAddress> &dests, vector<ezAddress> &srcs);
  void ret(vector<ezAddress> &srcs);
  void call(ezAddress &func, vector<ezAddress> &args,
                   vector<ezAddress> &rets);
  void thd(ezAddress &func, vector<ezAddress> &args,
                   vector<ezAddress> &rets, ezAddress &handle);
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
  ezStackFrame(ezCarousel *crsl, ezStackFrameCallback* callback, ezTable<string, ezValue *> &globals,
               vector<ezValue *> &constants, ezALU &alu, ezGC &gc);
  /**
  * @brief is a destructor.
  */
  ~ezStackFrame();
  void step(void); // the arguments are temporary ones.
  void update(ezStackFrame* sf);
  void on_mark(void);
};
