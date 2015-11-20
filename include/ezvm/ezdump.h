#pragma once
#include "ezaddr.h"
#include "ezval.h"
#include "ezfile.h"
#include "ezasm.h"
#include <string>
#include <vector>

using namespace std;

class ezDump {
	private:
		ezAddress& m_entry;
		vector<ezValue*>& m_constants;
		vector< vector<ezValue*>* >& m_globals;
		ezASM* m_asm;
		void dump(ezFile& sink, const ezValue* v);
	public:
		ezDump(ezAddress& entry, vector<ezValue*>& constants, vector< vector<ezValue*>* >& globals, ezASM* pasm);
		void dump(const string path);
};
