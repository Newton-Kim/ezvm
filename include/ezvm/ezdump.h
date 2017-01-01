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
#include "ezasm.h"
#include <string>
#include <vector>

using namespace std;

/**
* @brief ezDump dumps the state of ezVM to a file system.
*/
class ezDump {
 private:
  /**
  * @brief A reference to a procedure entry point
  */
  ezAddress& m_entry;
  /**
  * @brief A reference to a constant segment
  */
  vector<ezValue*>& m_constants;
  /**
  * @brief A reference to a global segment
  */
  vector<ezValue*>& m_globals;
  /**
  * @brief A reference to ezASM insance
  */
  ezASM* m_asm;
  /**
  * @brief dumps the content of v to sink.
  *
  * @param sink is a file stream.
  * @param v is a value.
  */
  void dump(ezFile& sink, const ezAddress addr);
  void dump(ezFile& sink, const ezValue* v);

 public:
  /**
  * @brief is a constructor.
  *
    * @param entry is a reference to an entry point.
    * @param constants is a reference to a constant memory.
    * @param globals is a reference to a global memory.
  * @param pasm is a pointer to an ezASM instance.
  */
  ezDump(ezAddress& entry, vector<ezValue*>& constants,
         vector<ezValue*>& globals, ezASM* pasm);
  /**
  * @brief dumps the state of ezVM to a file of path.
  *
  * @param path is file name.
  */
  void dump(const string path);
};
