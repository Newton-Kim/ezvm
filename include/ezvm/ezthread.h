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
		vector< vector<size_t>* >& m_globals;
		stack<ezStackFrame*> m_stack;

	public:
		ezThread(ezAddress entry, vector< vector<size_t>* >& globals, vector<ezValue*>& constants);
		~ezThread();
		ezStepState step(void);
};
