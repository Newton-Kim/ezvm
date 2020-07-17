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

#include "ezasm_proc.h"
#include "ezvm/ezaddr.h"
#include "ezvm/ezalu.h"
#include "ezvm/ezgc.h"
#include "ezvm/ezobject.h"
#include "ezvm/eztable.h"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

using namespace std;

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
  vector<ezObject *> &m_constants;
  /**
   * @brief A reference to a global segment
   */
  ezTable<string, ezObject *> &m_globals;
  ezGC &m_gc;
  map<size_t, ezTable<string, ezObject *> *> m_scopes;

public:
  /**
   * @brief is a constructor.
   *
   * @param entry is a reference to an entry point.
   * @param constants is a reference to a constant memory.
   * @param globals is a reference to a global memory.
   */
  ezASM(ezAddress &entry);
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
  void load_intrinsics(char **symtab, ezObject **constants);
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
  ezAsmProcedure *new_proc(const string name, bool scope);
  /**
   * @brief finds the offset of a global segment whose name is value.
   *
   * @param value is a symbol.
   *
   * @return
   */
  size_t global(const string value);
  bool is_global(const string value);
  /**
   * @brief adds a null.
   *
   * @param value is a null.
   *
   * @return is an offset in a constant segment.
   */
  size_t constant_null(void);
  /**
   * @brief adds a constant value object.
   *
   * @param value is a string.
   *
   * @return is an offset in a constant segment.
   */
  size_t constant(ezALU *alu, ezValue *value);
};
