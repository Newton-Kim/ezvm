#pragma once

#include "ezval.h"
#include "ezaddr.h"
#include <vector>
#include <stack>

using namespace std;

enum ezStepState {
	EZ_STEP_CONTINUE,
	EZ_STEP_DONE
};

struct ezStackFrame {
	size_t pc;
	vector<ezValue*> local;
	vector<ezValue*> retruns;
	vector<ezAddress> return_dest;
	ezCarousel* carousel;
	ezStackFrame(ezCarousel* crsl);
	~ezStackFrame();
};

