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
#include <cstddef>
#include <vector>

using namespace std;

class ezThread {
	private:
		ezAddress m_entry;
		vector<ezValue*>& m_constants;
		vector<ezValue*> m_args;
		vector<ezValue*> m_rets;
		vector< vector<ezValue*>* >& m_globals;
		stack<ezStackFrame*> m_stack;
		ezValue* addr2val(ezAddress addr);
		ezValue* val2addr(ezAddress addr, ezValue* v);
		void val2addr(vector<ezAddress>& addr, vector<ezValue*>& vals);
		void call(ezNativeCarousel* func, uint8_t nargs, uint8_t nrets);
		void call(ezCarousel* func, uint8_t nargs, uint8_t nrets);

	public:
		ezThread(ezAddress entry, vector< vector<ezValue*>* >& globals, vector<ezValue*>& constants);
		~ezThread();
		ezStepState step(void);
		void mv(uint8_t ndsts, uint8_t nsrcs);
		void ld(void);
		void call(uint8_t nargs, uint8_t nrets);
		void add(uint8_t ndests, uint8_t nsrcs);
		void sub(uint8_t ndests, uint8_t nsrcs);
		void beq(uint8_t index);
		void blt(uint8_t index);
		void bra(uint8_t index);
};
