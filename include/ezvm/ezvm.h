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
#include "ezasm.h"
#include "ezarchive.h"
#include "ezdump.h"
#include "ezval.h"
#include "ezthread.h"
#include "ezgc.h"
#include <string>

using namespace std;

/**
* @brief ezVM is the VM class
*/
class ezVM : public ezGCClient{
 private:
  ezAddress m_entry;
  vector<ezValue*> m_constants;
  vector<ezValue*> m_globals;
  ezASM* m_pasm;
  ezArchive* m_parchive;
  ezDump* m_pdump;
  ezGC<ezValue> m_gc;
  ezALU m_alu;
  vector<ezThread*> m_threads;

 public:
  ezVM();
  ~ezVM();
  void run(void);
  ezASM& assembler(void);
  ezArchive& archive(void);
  ezDump& dump(void);
  void on_mark(void);
};
