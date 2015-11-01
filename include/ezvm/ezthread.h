#pragma once

#include "ezaddr.h"
#include "ezval.h"
#include <cstddef>
#include <vector>

using namespace std;

enum ezThreadState {
	EZ_THREAD_CONTINUE,
	EZ_THREAD_DONE
};

class ezThread {
	private:
		ezAddress m_entry;
		vector<ezValue*>& m_constants;
		vector< vector<size_t>* >& m_globals;

	public:
		ezThread(ezAddress entry, vector< vector<size_t>* > globals, vector<ezValue*> constants);
		ezThreadState step(void);
};
