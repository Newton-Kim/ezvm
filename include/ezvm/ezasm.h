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
#include "ezfile.h"
#include <cstddef>
#include <vector>
#include <string>
#include <map>

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
  ezCarousel* m_carousel;

 public:
  /**
  * @brief is a constructor.
  *
  * @param carousel is a pointer to a carousel.
  */
  ezAsmProcedure(ezCarousel* carousel);
  /**
  * @brief creates a call instruction. See add of ezas.
  *
  * @param func An address of a function to call
  * @param args Arguments which are passed to the function
  * @param rets Addresses which the return values of the function are stored.
  */
  void call(const ezAddress& func, vector<ezAddress>& args,
            vector<ezAddress>& rets);
  /**
  * @brief creates a mv instruction. See mv of ezas.
  *
  * @param dest Destination addresses which the values of src are stored.
  * @param src Addresses of source values.
  */
  void mv(vector<ezAddress>& dest, vector<ezAddress>& src);
  /**
  * @brief creates a ld instruction. See ld of ezas.
  *
  * @param dest An address which the loaded value is stored.
  * @param obj An address of an object whose member is loaded.
  * @param offset An offset of a member in the obj.
  */
  void ld(const ezAddress dest, const ezAddress obj, const ezAddress offset);
  /**
  * @brief creates an add instruction. See add of ezas.
  *
  * @param dest An address which the sum is stored.
  * @param src Addresses of addends.
  */
  void add(const ezAddress dest, vector<ezAddress>& src);
  /**
  * @brief creates an add instruction. The instruction stores a condition at the
  * address of cond. See add of ezas.
  *
  * @param dest An address which the sum is stored.
  * @param cond An address which the condition is stored.
  * @param src Addresses of addends.
  */
  void add(const ezAddress dest, const ezAddress cond, vector<ezAddress>& src);
  /**
  * @brief creates a bitwise AND instruction. See and of ezas.
  *
  * @param dest An address which the result is stored.
  * @param larg A left argument.
  * @param rarg A right argument.
  */
  void bitwise_and(const ezAddress dest, const ezAddress larg,
                   const ezAddress rarg);
  /**
  * @brief creates a bitwise AND instruction. The instruction stores a condition
  *at the address of cond. See and of ezas.
  *
  * @param dest An address which the result is stored.
  * @param cond An address which the condition is stored.
  * @param larg A left argument.
  * @param rarg A right argument.
  */
  void bitwise_and(const ezAddress dest, const ezAddress cond,
                   const ezAddress larg, const ezAddress rarg);
  /**
  * @brief creates an conditional branching instruction. See beq of ezas.
  *
  * @param cond is a condition left by respective instruction.
  * @param label is a tag which the process count jumps when cond meets
  * zero-condition.
  */
  void beq(const ezAddress cond, string label);
  /**
  * @brief creates an conditional branching instruction. See blt of ezas.
  *
  * @param cond is a condition left by respective instruction.
  * @param label is a tag which the process count jumps when cond meets
  * negative-condition.
  */
  void blt(const ezAddress cond, string label);
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
  * @param src Addresses of addends.
  */
  void div(const ezAddress dest, vector<ezAddress>& src);
  /**
  * @brief creates an div instruction. The instruction stores a condition at the
  * address of cond. See div of ezas.
  *
  * @param dest An address which the product is stored.
  * @param cond An address which the condition is stored.
  * @param src Addresses of addends.
  */
  void div(const ezAddress dest, const ezAddress cond, vector<ezAddress>& src);
  /**
  * @brief tags the address with the name.
  *
  * @param name is a symbol of the address.
  */
  void label(string name);
  /**
  * @brief creates a mul instruction. See mul of ezas.
  *
  * @param dest An address which the product is stored.
  * @param src Addresses of addends.
  */
  void mul(const ezAddress dest, vector<ezAddress>& src);
  /**
  * @brief creates a mul instruction. The instruction stores a condition at the
  * address of cond. See mul of ezas.
  *
  * @param dest An address which the product is stored.
  * @param cond An address which the condition is stored.
  * @param src Addresses of addends.
  */
  void mul(const ezAddress dest, const ezAddress cond, vector<ezAddress>& src);
/**
* @brief creates a neg instruction. See neg of ezas.
*
  * @param dest An address which the negate is stored.
* @param src An address of the original value.
*/
  void neg(const ezAddress dest, ezAddress src);
/**
* @brief creates a neg instruction. The instruction stores a condition at the address of cond. See neg of ezas.
*
* @param dest An address which the negate is stored.
  * @param cond An address which the condition is stored.
* @param src An address of the original value.
*/
  void neg(const ezAddress dest, const ezAddress cond, ezAddress src);
  /**
  * @brief creates a bitwise OR instruction. See and of ezas.
  *
  * @param dest An address which the result is stored.
  * @param larg A left argument.
  * @param rarg A right argument.
  */
  void bitwise_or(const ezAddress dest, const ezAddress larg,
                   const ezAddress rarg);
  /**
  * @brief creates a bitwise OR instruction. The instruction stores a condition
  *at the address of cond. See or of ezas.
  *
  * @param dest An address which the result is stored.
  * @param cond An address which the condition is stored.
  * @param larg A left argument.
  * @param rarg A right argument.
  */
  void bitwise_or(const ezAddress dest, const ezAddress cond,
                   const ezAddress larg, const ezAddress rarg);
  /**
  * @brief creates a subtraction instruction. See sub of ezas.
  *
  * @param dest An address which the difference is stored.
  * @param src Addresses of minuend and subtrahend.
  */
  void sub(const ezAddress dest, vector<ezAddress>& src);
  /**
  * @brief creates an subtration instruction. The instruction stores a condition
  * at the address of cond. See sub of ezas.
  *
  * @param dest An address which the difference is stored.
  * @param src Addresses of minuend and subtrahend.
  */
  void sub(const ezAddress dest, const ezAddress cond, vector<ezAddress>& src);
  /**
  * @brief creates a bitwise XOR instruction. See xor of ezas.
  *
  * @param dest An address which the result is stored.
  * @param larg A left argument.
  * @param rarg A right argument.
  */
  void bitwise_xor(const ezAddress dest, const ezAddress larg,
                   const ezAddress rarg);
  /**
  * @brief creates a bitwise XOR instruction. The instruction stores a condition
  *at the address of cond. See xor of ezas.
  *
  * @param dest An address which the result is stored.
  * @param cond An address which the condition is stored.
  * @param larg A left argument.
  * @param rarg A right argument.
  */
  void bitwise_xor(const ezAddress dest, const ezAddress cond,
                   const ezAddress larg, const ezAddress rarg);
};

/**
* @brief ezASM alters the states of ezVM.
*/
class ezASM {
 private:
  /**
  * @brief A reference to a procedure entry point
  */
  ezAddress& m_entry;
  /**
  * @brief A symbol of the entry point
  */
  string m_entry_string;
  /**
  * @brief A reference to a constant segment
  */
  vector<ezValue*>& m_constants;
  /**
  * @brief A reference to a global segment
  */
  vector<vector<ezValue*>*>& m_globals;
  /**
  * @brief A symbol table of segments
  */
  map<string, size_t> m_seg_symtab;
  /**
  * @brief A collection of symbol tables respect to segments
  */
  vector<map<string, size_t>*> m_offset_symtab;

 public:
  /**
  * @brief is a constructor.
  *
  * @param entry is a reference to an entry point.
  * @param constants is a reference to a constant memory.
  * @param globals is a reference to a global memory.
  */
  ezASM(ezAddress& entry, vector<ezValue*>& constants,
        vector<vector<ezValue*>*>& globals);
  /**
  * @brief is a destructor.
  */
  ~ezASM();
  /**
  * @brief specifies a library.
  *
  * @param name is a symbol of the library.
  */
  void import(const string name);
  /**
  * @brief sets an entry point.
  *
  * @param name is a symbol of the entry point.
  */
  void entry(const string name);
  /**
  * @brief creates a new procedure and an assembler dedicated to the procedure.
  *
  * @param name is the symbol of a procedure.
  * @param argc is the number of the arguments.
  * @param retc is the number of the returns.
  * @param mems is the size of memories.
  *
  * @return is a pointer to the assember.
  */
  ezAsmProcedure* new_proc(const string name, int argc, int retc, int mems);
  /**
  * @brief finds the offset of a global segment whose name is value.
  *
  * @param value is a symbol.
  *
  * @return
  */
  size_t global(const string value);
  /**
  * @brief adds a constant string.
  *
  * @param value is a string.
  *
  * @return is an offset in a constant segment.
  */
  size_t constant(const char* value);
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
  * @brief finds the offset of a segment segment whose name is value.
  *
  * @param segment is a name of a segment.
  *
  * @param value is a symbol.
  *
  * @return
  */
  size_t offset(const string segment, const string value);
  /**
  * @brief finds the segment index whose name is 'segment'.
  *
  * @param segment is the symbol of a segment.
  *
  * @return is the index of the segemnt.
  */
  size_t segment(const string segment);
  /**
  * @brief dumps the states and the instructions to sink.
  *
  * @param sink is a file handle that the state data is dumped.
  */
  void dump(ezFile& sink);
};
