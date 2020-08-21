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

#include "ezvm/ezaddr.h"
#include "ezvm/ezalu.h"
#include "ezvm/ezfile.h"
#include "ezvm/ezinstruction.h"
#include "ezvm/ezval.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class ezAsmInstruction {
  friend class ezAsmProcedure;

private:
  ezALU *m_alu;
  vector<ezInstruction *> m_instruction;

public:
  ezAsmInstruction(ezALU *alu);
  virtual ~ezAsmInstruction();
  /**
   * @brief creates an add instruction. See add of ezas.
   *
   * @param dest An address which the sum is stored.
   * @param lsrc Addresses of left addends.
   * @param rsrc Addresses of right addends.
   */
  void add(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  /**
   * @brief creates an add instruction. See add of ezas.
   *
   * @param dest An address which the sum is stored.
   * @param cond An address which the condition is stored.
   * @param lsrc Addresses of left addends.
   * @param rsrc Addresses of right addends.
   */
  void add(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  /**
   * @brief creates a bitwise AND instruction. See and of ezas.
   *
   * @param dest An address which the result is stored.
   * @param src Addresses of arguments.
   */
  void bitwise_and(const ezAddress dest, const ezAddress &lsrc,
                   const ezAddress &rsrc);
  /**
   * @brief creates a bitwise AND instruction. The instruction stores a
   *condition at the address of cond. See and of ezas.
   *
   * @param dest An address which the result is stored.
   * @param cond An address which the condition is stored.
   * @param src Addresses of arguments.
   */
  void bitwise_and(const ezAddress dest, const ezAddress cond,
                   const ezAddress &lsrc, const ezAddress &rsrc);
  /**
   * @brief creates an conditional branching instruction. See beq of ezas.
   *
   * @param cond is a condition left by respective instruction.
   * @param offset is a tag which the process count jumps when cond meets
   * zero-condition.
   */
  void beq(const ezAddress cond, size_t offset);
  /**
   * @brief creates an conditional branching instruction. See bge of ezas.
   *
   * @param cond is a condition left by respective instruction.
   * @param offset is a tag which the process count jumps when cond meets
   * greather or equal-condition.
   */
  void bge(const ezAddress cond, size_t offset);
  /**
   * @brief creates an conditional branching instruction. See blt of ezas.
   *
   * @param cond is a condition left by respective instruction.
   * @param offset is a tag which the process count jumps when cond meets
   * negative-condition.
   */
  void blt(const ezAddress cond, size_t offset);
  /**
   * @brief creates an conditional branching instruction. See bne of ezas.
   *
   * @param cond is a condition left by respective instruction.
   * @param offset is a tag which the process count jumps when cond meets
   * non-zero-condition.
   */
  void bne(const ezAddress cond, size_t offset);
  /**
   * @brief creates an unconditional branching instruction. See bra of ezas.
   *
   * @param offset is a tag which the process count jumps.
   */
  void bra(size_t offset);
  /**
   * @brief creates a call instruction. See call of ezas.
   *
   * @param func An address of a function to call
   * @param args Arguments which are passed to the function
   * @param ret Addresses which the return value of the function is stored.
   */
  void call(const ezAddress &func, vector<ezAddress> &args, ezAddress &ret);
  /**
   * @brief creates a call instruction. See call of ezas.
   *
   * @param func An address of a function to call
   * @param args Arguments which are passed to the function
   */
  void call(const ezAddress &func, vector<ezAddress> &args);
  void cmp(const ezAddress &cond, const ezAddress &larg, const ezAddress &rarg);
  /**
   * @brief creates a mv instruction. See mv of ezas.
   *
   * @param dest Destination addresses which the values of src are stored.
   * @param src Addresses of source values.
   */
  /**
   * @brief creates an div instruction. See div of ezas.
   *
   * @param dest An address which the product is stored.
   * @param lsrc Addresses of dividend.
   * @param rsrc Addresses of dividor.
   */
  void div(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  /**
   * @brief creates an div instruction. See div of ezas.
   *
   * @param dest An address which the product is stored.
   * @param cond An address which the condition is stored.
   * @param lsrc Addresses of dividend.
   * @param rsrc Addresses of dividor.
   */
  void div(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  void fgc(void);
  /**
   * @brief creates an pow instruction. See pow of ezas.
   *
   * @param dest An address which the sum is stored.
   * @param lsrc Addresses of base.
   * @param rsrc Addresses of exponent.
   */
  void lsl(const ezAddress dest, const ezAddress obj, const ezAddress offset);
  void lsl(const ezAddress dest, const ezAddress cond, const ezAddress obj,
           const ezAddress offset);
  void lsr(const ezAddress dest, const ezAddress obj, const ezAddress offset);
  void lsr(const ezAddress dest, const ezAddress cond, const ezAddress obj,
           const ezAddress offset);
  /**
   * @brief creates a mod instruction. See mod of ezas.
   *
   * @param dest An address which the product is stored.
   * @param lsrc Addresses of the dividend.
   * @param rsrc Addresses of the divisor.
   */
  void mod(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  /**
   * @brief creates a mod instruction. See mod of ezas.
   *
   * @param dest An address which the product is stored.
   * @param cond An address which the condition is stored.
   * @param lsrc Addresses of the dividend.
   * @param rsrc Addresses of the divisor.
   */
  void mod(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  /**
   * @brief creates a mul instruction. See mul of ezas.
   *
   * @param dest An address which the product is stored.
   * @param lsrc Addresses of multiplicand.
   * @param rsrc Addresses of multiplier.
   */
  void mul(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  /**
   * @brief creates a mul instruction. See mul of ezas.
   *
   * @param dest An address which the product is stored.
   * @param cond An address which the condition is stored.
   * @param lsrc Addresses of multiplicand.
   * @param rsrc Addresses of multiplier.
   */
  void mul(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  void mv(vector<ezAddress> &dest, vector<ezAddress> &src);
  void mv(ezAddress &dest, ezAddress &src);
  /**
   * @brief creates a neg instruction. See neg of ezas.
   *
   * @param dest An address which the negate is stored.
   * @param src An address of the original value.
   */
  void neg(const ezAddress dest, ezAddress src);
  /**
   * @brief creates a neg instruction. The instruction stores a condition at the
   *address of cond. See neg of ezas.
   *
   * @param dest An address which the negate is stored.
   * @param cond An address which the condition is stored.
   * @param src An address of the original value.
   */
  void neg(const ezAddress dest, const ezAddress cond, ezAddress src);
  /**
   * @brief creates a bitwise NOT instruction. See not of ezas.
   *
   * @param dest An address which the product is stored.
   * @param src An address of the original value.
   */
  void bitwise_not(const ezAddress dest, ezAddress src);
  /**
   * @brief creates a bitwise NOT instruction. The instruction stores a
   *condition at the address of cond. See not of ezas.
   *
   * @param dest An address which the product is stored.
   * @param cond An address which the condition is stored.
   * @param src An address of the original value.
   */
  void bitwise_not(const ezAddress dest, const ezAddress cond, ezAddress src);
  /**
   * @brief creates a bitwise OR instruction. See and of ezas.
   *
   * @param dest An address which the result is stored.
   * @param src Addresses of arguments.
   */
  void bitwise_or(const ezAddress dest, const ezAddress &lsrc,
                  const ezAddress &rsrc);
  /**
   * @brief creates a bitwise OR instruction. The instruction stores a condition
   *at the address of cond. See or of ezas.
   *
   * @param dest An address which the result is stored.
   * @param cond An address which the condition is stored.
   * @param src Addresses of arguments.
   */
  void bitwise_or(const ezAddress dest, const ezAddress cond,
                  const ezAddress &lsrc, const ezAddress &rsrc);
  void powv(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  /**
   * @brief creates an add instruction. See pow of ezas.
   *
   * @param dest An address which the sum is stored.
   * @param cond An address which the condition is stored.
   * @param lsrc Addresses of base.
   * @param rsrc Addresses of exponent.
   */
  void powv(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
            const ezAddress &rsrc);
  void ret(void);
  void ret(vector<ezAddress> &src);
  /**
   * @brief creates a subtraction instruction. See sub of ezas.
   *
   * @param dest An address which the difference is stored.
   * @param lsrc Addresses of minuend.
   * @param rsrc Addresses of subtrahend.
   */
  void sub(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  /**
   * @brief creates a subtraction instruction. See sub of ezas.
   *
   * @param dest An address which the difference is stored.
   * @param lsrc Addresses of minuend.
   * @param rsrc Addresses of subtrahend.
   */
  void sub(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  void teq(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  void teq(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  void tge(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  void tge(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  /**
   * @brief creates a thd instruction. See thd of ezas.
   *
   * @param func An address of a function to call
   * @param args Arguments which are passed to the function
   * @param rets An address to the handle (1st arguemtn) and addresses which the
   * return values of the function are stored.
   */
  void thd(const ezAddress &func, vector<ezAddress> &args, ezAddress &ret,
           const ezAddress &handle);
  void thd(const ezAddress &func, vector<ezAddress> &args,
           const ezAddress &handle);
  void tlt(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  void tlt(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  void tne(const ezAddress dest, const ezAddress &lsrc, const ezAddress &rsrc);
  void tne(const ezAddress dest, const ezAddress cond, const ezAddress &lsrc,
           const ezAddress &rsrc);
  void wait(const ezAddress &handle);
  /**
   * @brief creates a bitwise XOR instruction. See xor of ezas.
   *
   * @param dest An address which the result is stored.
   * @param src Addresses of arguments.
   */
  void bitwise_xor(const ezAddress dest, const ezAddress &lsrc,
                   const ezAddress &rsrc);
  /**
   * @brief creates a bitwise XOR instruction. The instruction stores a
   *condition at the address of cond. See xor of ezas.
   *
   * @param dest An address which the result is stored.
   * @param cond An address which the condition is stored.
   * @param src Addresses of arguments.
   */
  void bitwise_xor(const ezAddress dest, const ezAddress cond,
                   const ezAddress &lsrc, const ezAddress &rsrc);
  void user_command(ezInstruction *inst);
  size_t size(void) { return m_instruction.size(); }
};
