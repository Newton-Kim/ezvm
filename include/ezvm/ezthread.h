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

	public:
		ezThread(ezAddress entry, vector< vector<ezValue*>* >& globals, vector<ezValue*>& constants);
		~ezThread();
		ezStepState step(void);
		void mv(uint8_t ndsts, uint8_t nsrcs);
		void ld(void);
		void call(uint8_t nargs, uint8_t nrets);
};
