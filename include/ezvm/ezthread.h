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
		void mv(uint8_t arg1, uint8_t arg2, uint8_t arg3);
		void ld(uint8_t arg1, uint8_t arg2, uint8_t arg3);
		void call(uint8_t arg1, uint8_t arg2, uint8_t arg3);
};
