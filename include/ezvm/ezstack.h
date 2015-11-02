#pragma once

#include "ezval.h"
#include <vector>
#include <stack>

using namespace std;

enum ezStepState {
	EZ_STEP_CONTINUE,
	EZ_STEP_DONE
};

class ezStackFrame {
	private:
		size_t m_pc;
		vector<ezValue*> m_local;
		ezCarousel* m_carousel;
		ezNativeCarousel* m_native_carould;
		vector<ezValue*> m_args;
		vector<ezValue*>& m_rets;
	public:
		ezStackFrame(ezCarousel* carousel, vector<ezValue*>& args, vector<ezValue*>& rets);
		ezStackFrame(ezNativeCarousel* carousel, vector<ezValue*>& args, vector<ezValue*>& rets);
		ezStepState step(void);
};

