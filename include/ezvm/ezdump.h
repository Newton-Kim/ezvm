#pragma once
#include "ezaddr.h"
#include "ezval.h"
#include "ezfile.h"
#include <string>
#include <vector>

using namespace std;

class ezDump {
	private:
		ezAddress& m_entry;
		vector<ezValue*>& m_constants;
		vector< vector<ezValue*>* >& m_globals;
		void dump(ezFile& sink, const ezValue* v);
	public:
		ezDump(ezAddress& entry, vector<ezValue*>& constants, vector< vector<ezValue*>* >& globals);
		void dump(const string path);
};
