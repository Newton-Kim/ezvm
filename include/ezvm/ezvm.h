#pragma once
#include "ezasm.h"
#include "ezarchive.h"
#include "ezval.h"
#include <string>

using namespace std;

class ezVM {
	private:
		string m_entry;
		vector<ezValue*> m_constants;
		vector<ezAddress*> m_globals;
		ezASM* m_pasm;
		ezArchive* m_parchive;
	public:
		ezVM();
		~ezVM();
		void run(void);
		ezASM& assembler(void);
		ezArchive& archive(void);
};
