#pragma once
#include "ezvm/ezval.h"
#include <string>

using namespace std;

class ezIntrinsic {
	public:
		static void load(const string name, char*** symtab, ezValue*** constants);
};
