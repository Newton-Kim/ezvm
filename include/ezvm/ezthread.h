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
#include "ezval.h"
#include "ezstack.h"
#include "ezalu.h"
#include <cstddef>
#include <vector>
#include <functional>

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
  vector<ezValue*>& m_constants;
  /**
  * @brief An arguments to a new stack frame
  */
  vector<ezValue*> m_args;
  /**
  * @brief A return values from the stack frame on top
  */
  vector<ezValue*> m_rets;
  /**
  * @brief A reference to a global segment
  */
  vector<ezValue*>& m_globals;
  /**
  * @brief A stack of stack frames
  */
  stack<ezStackFrame*> m_stack;
  /**
  * @brief An arithematic logic unit
  */
  ezALU& m_alu;
  /**
  * @brief fetches a value from an address.
  *
  * @param addr is an address.
  *
  * @return A value.
  */
  ezValue* addr2val(ezAddress addr);
  /**
  * @brief places a value at an address.
  *
  * @param addr is an address to place v.
  * @param v is a value.
  */
  void val2addr(ezAddress addr, ezValue* v);
  /**
  * @brief places values at respective addresses.
  *
  * @param addr is an array of addresses.
  * @param v is an array of values.
  */
  void val2addr(vector<ezAddress>& addr, vector<ezValue*>& vals);
  /**
  * @brief invokes a native carousel.
  *
  * @param func is a pointer to a native carousel.
  * @param nargs is a number of arguments which follows the func.
  * @param nrets is a number of return addresses which follows the arguments.
  */
  void call(ezNativeCarousel* func, uint8_t nargs, uint8_t nrets);
  /**
  * @brief invokes a carousel.
  *
  * @param func is a pointer to a native carousel.
  * @param nargs is a number of arguments which follows the func.
  * @param nrets is a number of return addresses which follows the arguments.
  */
  void call(ezCarousel* func, uint8_t nargs, uint8_t nrets);
  void binary_operation(uint8_t nargs, uint8_t nsrcs,
                        function<ezValue*(ezValue*, ezValue*)> binary_func,
                        function<ezValue*(vector<ezValue*>&)> multi_func);
  void unary_operation(uint8_t nargs, uint8_t nsrcs,
                       function<ezValue*(ezValue*)> unary_func);
  void conditional_bra(uint8_t index, function<bool(ezCondition*)> func);
  void shift_operation(uint8_t ndests, uint8_t nsrcs, uint8_t noffsets,
                       function<ezValue*(ezValue*, ezValue*)> func);

 public:
  /**
  * @brief constructs a thread with an entry point, a global memory, and a
  *constant memory.
  *
    * @param entry is a reference to an entry point.
    * @param globals is a reference to a global memory.
    * @param constants is a reference to a constant memory.
  */
  ezThread(ezAddress entry, vector<ezValue*>& globals,
           vector<ezValue*>& constants, ezALU& alu);
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
  void mv(uint8_t ndsts, uint8_t nsrcs);
  void ld(void);
  void lsl(uint8_t ndests, uint8_t nsrcs, uint8_t offsets);
  void lsr(uint8_t ndests, uint8_t nsrcs, uint8_t offsets);
  void call(uint8_t nargs, uint8_t nrets);
  void cmp(uint8_t ndests, uint8_t nsrcs);
  void add(uint8_t ndests, uint8_t nsrcs);
  void bitwise_and(uint8_t ndests, uint8_t nsrcs);
  void div(uint8_t ndests, uint8_t nsrcs);
  void sub(uint8_t ndests, uint8_t nsrcs);
  void beq(uint8_t index);
  void bge(uint8_t index);
  void blt(uint8_t index);
  void bne(uint8_t index);
  void bra(uint8_t index);
  void mod(uint8_t ndests, uint8_t nsrcs);
  void mul(uint8_t ndests, uint8_t nsrcs);
  void neg(uint8_t ndests, uint8_t nsrcs);
  void bitwise_not(uint8_t ndests, uint8_t nsrcs);
  void bitwise_or(uint8_t ndests, uint8_t nsrcs);
  void ret(uint8_t nsrcs);
  void bitwise_xor(uint8_t ndests, uint8_t nsrcs);
};
