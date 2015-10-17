#pragma once
#include "ezfile.h"
#include <string>

using namespace std;

class ezLog {
	public:
		static void initialize(const string target);
		static ezFile& logger(void);
};
