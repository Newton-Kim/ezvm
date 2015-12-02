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
		void bra(uint8_t index);
};
