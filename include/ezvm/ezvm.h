#pragma once
#include "ezasm.h"
#include "ezarchive.h"
#include <string>

using namespace std;

class ezVM {
	private:
		ezASM* m_pasm;
		ezArchive* m_parchvie;
	public:
		void run(void);
		ezASM& assembler(void);
		ezArchive& archive(void);
};
