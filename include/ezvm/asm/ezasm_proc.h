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

#include "ezvm/asm/ezasm_instr.h"
#include "ezvm/ezfunc.h"
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief ezAsmProcedure fills in the instance of ezFunction with respective
 * instructions.
 */
class ezAsmProcedure {
private:
  /**
   * @brief is an instance of a carousel.
   */
  ezFunction *m_carousel;
  size_t m_local_index;
  map<string, size_t> m_locals;
  map<string, size_t> *m_scope;
  list<string> m_unresolved;

  bool search_unresolved(string label);

public:
  /**
   * @brief is a constructor.
   *
   * @param carousel is a pointer to a carousel.
   */
  ezAsmProcedure(ezFunction *carousel, ezAsmProcedure *parent);
  void args(size_t args);
  /**
   * @brief mems the local memory.
   *
   * @param mems is a new size. if the size of the local memory is greater than
   * mems. the size is not changed.
   */
  void mems(size_t mems);
  /**
   * @brief temps the local memory.
   *
   * @param temps is a new size. if the size of the local memory is greater than
   * temps. the size is not changed.
   */
  void temps(size_t temps);
  size_t local(const string value);
  /**
   * @brief tags the address with the name.
   *
   * @param name is a symbol of the address.
   * @param offset is an offset of the instruction array.
   */
  void label(string name, size_t offset);
  /**
   * @brief converts a local variable to respective slot index of a local
   * segment.
   *
   * @param label
   *
   * @return The offset of a local segment
   */
  size_t label2index(string label);
  void append_instruction(ezAsmInstruction *instr);
  void validate(void);
};
