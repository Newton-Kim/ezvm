#pragma once
#include "ezasm.h"
#include "ezarchive.h"
#include "ezdump.h"
#include "ezval.h"
#include "ezthread.h"
#include <string>

using namespace std;

class ezVM {
	private:
		ezAddress m_entry;
		vector<ezValue*> m_constants;
		vector< vector<ezValue*>* > m_globals;
		ezASM* m_pasm;
		ezArchive* m_parchive;
		ezDump* m_pdump;
		vector<ezThread*> m_threads;
	public:
		ezVM();
		~ezVM();
		void run(void);
		ezASM& assembler(void);
		ezArchive& archive(void);
		ezDump& dump(void);
};
