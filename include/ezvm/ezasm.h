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
#include "ezfile.h"
#include "ezgc.h"
#include "ezinstruction.h"
#include "eztable.h"
#include "ezval.h"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

using namespace std;

/**
* @brief ezAsmProcedure fills in the instance of ezCarousel with respective
* instructions.
*/
class ezAsmProcedure {
private:
  /**
  * @brief converts a local variable to respective slot index of a local
  * segment.
  *
  * @param label
  *
  * @return The offset of a local segment
  */
  size_t label2index(string label);
  /**
  * @brief is an instance of a carousel.
  */
  ezCarousel *m_carousel;
  void instruction_with_an_argument(ezOpCode op, const ezAddress dest,
                                    const ezAddress src);
  void instruction_with_an_argument(ezOpCode op, const ezAddress dest,
                                    const ezAddress cond, const ezAddress src);
  void instruction_with_1_1_1_arguments(const ezOpCode op, const ezAddress dest,
                                        const ezAddress obj,
                                        const ezAddress offset);
  void instruction_with_1_1_1_arguments(const ezOpCode op, const ezAddress dest,
                                        const ezAddress cond,
                                        const ezAddress obj,
                                        const ezAddress offset);
  void instruction_with_1_2_0_arguments(const ezOpCode op, const ezAddress dest,
                                        const ezAddress obj,
                                        const ezAddress offset);
  void instruction_with_1_2_0_arguments(const ezOpCode op, const ezAddress dest,
                                        const ezAddress cond,
                                        const ezAddress obj,
                                        const ezAddress offset);
  void branch_instruction(ezOpCode op, const ezAddress cond, string label);

public:
  /**
  * @brief is a constructor.
  *
  * @param carousel is a pointer to a carousel.
  */
  ezAsmProcedure(ezCarousel *carousel);
  /**
  * @brief grows the local memory.
  *
  * @param mems is a new size. if the size of the local memory is greater than
  * mems. the size is not changed.
  */
  void grow(size_t mems);
  /**
  * @brief creates a call instruction. See add of ezas.
  *
  * @param func An address of a function to call
  * @param args Arguments which are passed to the function
  * @param rets Addresses which the return values of the function are stored.
  */
  void call(const ezAddress &func, vector<ezAddress> &args,
            vector<ezAddress> &rets);
  void cmp(const ezAddress &cond, const ezAddress &larg, const ezAddress &rarg);
  /**
  * @brief creates a mv instruction. See mv of ezas.
  *
  * @param dest Destination addresses which the values of src are stored.
  * @param src Addresses of source values.
  */
  void mv(vector<ezAddress> &dest, vector<ezAddress> &src);
  void lsl(const ezAddress dest, const ezAddress obj, const ezAddress offset);
  void lsl(const ezAddress dest, const ezAddress cond, const ezAddress obj,
           const ezAddress offset);
  void lsr(const ezAddress dest, const ezAddress obj, const ezAddress offset);
  void lsr(const ezAddress dest, const ezAddress cond, const ezAddress obj,
           const ezAddress offset);
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
  * @brief creates a bitwise AND instruction. The instruction stores a condition
  *at the address of cond. See and of ezas.
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
  * @param label is a tag which the process count jumps when cond meets
  * zero-condition.
  */
  void beq(const ezAddress cond, string label);
  /**
  * @brief creates an conditional branching instruction. See bge of ezas.
  *
  * @param cond is a condition left by respective instruction.
  * @param label is a tag which the process count jumps when cond meets
  * greather or equal-condition.
  */
  void bge(const ezAddress cond, string label);
  /**
  * @brief creates an conditional branching instruction. See blt of ezas.
  *
  * @param cond is a condition left by respective instruction.
  * @param label is a tag which the process count jumps when cond meets
  * negative-condition.
  */
  void blt(const ezAddress cond, string label);
  /**
  * @brief creates an conditional branching instruction. See bne of ezas.
  *
  * @param cond is a condition left by respective instruction.
  * @param label is a tag which the process count jumps when cond meets
  * non-zero-condition.
  */
  void bne(const ezAddress cond, string label);
  /**
  * @brief creates an unconditional branching instruction. See bra of ezas.
  *
  * @param label is a tag which the process count jumps.
  */
  void bra(string label);
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
  * @brief tags the address with the name.
  *
  * @param name is a symbol of the address.
  */
  void label(string name);
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
  * @brief creates a bitwise NOT instruction. The instruction stores a condition
  *at the
  *address of cond. See not of ezas.
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
  /**
  * @brief creates a bitwise XOR instruction. See xor of ezas.
  *
  * @param dest An address which the result is stored.
  * @param src Addresses of arguments.
  */
  void bitwise_xor(const ezAddress dest, const ezAddress &lsrc,
                   const ezAddress &rsrc);
  /**
  * @brief creates a bitwise XOR instruction. The instruction stores a condition
  *at the address of cond. See xor of ezas.
  *
  * @param dest An address which the result is stored.
  * @param cond An address which the condition is stored.
  * @param src Addresses of arguments.
  */
  void bitwise_xor(const ezAddress dest, const ezAddress cond,
                   const ezAddress &lsrc, const ezAddress &rsrc);
};

/**
* @brief ezASM alters the states of ezVM.
*/
class ezASM {
private:
  /**
  * @brief A reference to a procedure entry point
  */
  ezAddress &m_entry;
  /**
  * @brief A symbol of the entry point
  */
  string m_entry_string;
  /**
  * @brief A reference to a constant segment
  */
  vector<ezValue *> &m_constants;
  /**
  * @brief A reference to a global segment
  */
  ezTable<string, ezValue *> &m_globals;
  ezGC &m_gc;
  map<size_t, ezTable<string, ezValue *> *> m_scopes;

public:
  /**
  * @brief is a constructor.
  *
  * @param entry is a reference to an entry point.
  * @param constants is a reference to a constant memory.
  * @param globals is a reference to a global memory.
  */
  ezASM(ezAddress &entry, vector<ezValue *> &constants,
        ezTable<string, ezValue *> &globals, ezGC &gc);
  /**
  * @brief is a destructor.
  */
  ~ezASM();
  /**
  * @brief load intrinsic functions.
  *
  * @param symtab is an array of symbols respective to constants.
  * @param constants is an array of intrinsic functions.
  */
  void load_intrinsics(char **symtab, ezValue **constants);
  /**
  * @brief sets an entry point.
  *
  * @param name is a symbol of the entry point.
  */
  void entry(const string name);
  /**
  * @brief sets the global memory slot.
  *
  * @param name is the name of the slot.
  */
  void reset(const string name);
  /**
  * @brief creates a new procedure and an assembler dedicated to the procedure.
  *
  * @param name is the symbol of a procedure.
  * @param argc is the number of the arguments.
  * @param mems is the size of memories.
  * @param scope is the identifier of the scope.
  * @param scpkey is the reference to the scope.
  *
  * @return is a pointer to the assember.
  */
  ezAsmProcedure *new_proc(const string name, int argc, size_t mems, int scope,
                           int scpkey);
  /**
  * @brief finds the offset of a global segment whose name is value.
  *
  * @param value is a symbol.
  *
  * @return
  */
  size_t global(const string value);
  /**
  * @brief adds a null.
  *
  * @param value is a null.
  *
  * @return is an offset in a constant segment.
  */
  size_t constant_null(void);
  /**
  * @brief adds a constant string.
  *
  * @param value is a string.
  *
  * @return is an offset in a constant segment.
  */
  size_t constant(const char *value);
  /**
  * @brief adds a constant integer number.
  *
  * @param value is an integer number.
  *
  * @return is an offset in a constant segment.
  */
  size_t constant(const int value);
  /**
  * @brief adds a constant boolean.
  *
  * @param value is a boolean.
  *
  * @return is an offset in a constant segment.
  */
  size_t constant(const bool value);
  /**
  * @brief adds a constant double.
  *
  * @param value is a double.
  *
  * @return is an offset in a constant segment.
  */
  size_t constant(const double value);
  /**
  * @brief adds a constant complex.
  *
  * @param value is a complex.
  *
  * @return is an offset in a constant segment.
  */
  size_t constant(const complex<double> value);
};
