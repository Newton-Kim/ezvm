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
#include <stack>
#include <vector>
#include <functional>

using namespace std;

/**
* @brief ezStepState shows if the stack has procedures to run.
*/
enum ezStepState {
  /**
  * @brief means the stack has procedures to run.
  */
  EZ_STEP_CONTINUE,
  /**
  * @brief means the stack is empty and ready to be collected.
  */
  EZ_STEP_CALL,
  EZ_STEP_DONE
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
  * @brief is a local segment.
  */
  vector<ezValue *> m_local;
  /**
  * @brief is a collection of return values.
  */
  vector<ezValue *> m_returns;
  /**
  * @brief is a collection of addresses where the result values place.
  */
  vector<ezAddress> m_return_dest;
  /**
  * @brief is a pointer to a carousel.
  */
  ezCarousel *m_carousel;
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
  ezStackFrame* m_callee;
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
  ezStepState call(ezNativeCarousel *func, uint8_t nargs, uint8_t nrets);
  /**
  * @brief invokes a carousel.
  *
  * @param func is a pointer to a native carousel.
  * @param nargs is a number of arguments which follows the func.
  * @param nrets is a number of return addresses which follows the arguments.
  */
  ezStepState call(ezCarousel *func, uint8_t nargs, uint8_t nrets);
  void binary_operation(
    uint8_t ndests, uint8_t nsrcs,
    function<ezValue *(ezValue *, ezValue *)> binary_func,
    function<ezValue *(vector<ezValue *> &)> multi_func);
  void conditional_bra(uint8_t index, function<bool(ezCondition *)> func);
  void
  shift_operation(uint8_t ndests, uint8_t nsrcs, uint8_t noffsets,
                  function<ezValue *(ezGC &gc, ezValue *, ezValue *)> func);
  void unary_operation(uint8_t nargs, uint8_t nsrcs,
                       function<ezValue *(ezValue *)> unary_func);
  void add(uint8_t ndests, uint8_t nsrcs);
  void bitwise_and(uint8_t ndests, uint8_t nsrcs);
  void beq(uint8_t index);
  void bge(uint8_t index);
  void blt(uint8_t index);
  void bne(uint8_t index);
  void bra(uint8_t index);
  ezStepState call(uint8_t nargs, uint8_t nrets);
  void cmp(uint8_t ndests, uint8_t nsrcs);
  void div(uint8_t ndests, uint8_t nsrcs);
  void lsl(uint8_t ndests, uint8_t nsrcs, uint8_t offsets);
  void lsr(uint8_t ndests, uint8_t nsrcs, uint8_t offsets);
  void mod(uint8_t ndests, uint8_t nsrcs);
  void mul(uint8_t ndests, uint8_t nsrcs);
  void mv(uint8_t ndsts, uint8_t nsrcs);
  void neg(uint8_t ndests, uint8_t nsrcs);
  void bitwise_not(uint8_t ndests, uint8_t nsrcs);
  void ret(uint8_t nsrcs);
  void sub(uint8_t ndests, uint8_t nsrcs);
  void bitwise_or(uint8_t ndests, uint8_t nsrcs);
  void bitwise_xor(uint8_t ndests, uint8_t nsrcs);

public:
  /**
  * @brief constructs the instance with a carousel.
  *
  * @param crsl is a pointer to a carousel.
  */
  ezStackFrame(ezCarousel *crsl, ezTable<string, ezValue *> &globals,
                   vector<ezValue *> &constants, ezALU &alu, ezGC &gc);
  /**
  * @brief is a destructor.
  */
  ~ezStackFrame();
  ezStepState step(void); //the arguments are temporary ones.
  ezStackFrame* callee(void) { return m_callee;}
  void on_mark(void);
};
