#pragma once

#include <cstddef>
#include <vector>
#include <string>

using namespace std;

class ezAddress {
	size_t segment;
	size_t offset;
};

class ezAsmProcedure {
	public:
		void call(const ezAddress func, vector<ezAddress>& args, vector<ezAddress>& rets);
		void mv(vector<ezAddress>& dest, vector<ezAddress>& src);
		void ld(const ezAddress dest, const ezAddress obj, const ezAddress offset);
};

class ezASM {
	public:
		void entry(const string name);
		ezAsmProcedure* new_proc(const string name, int argc, int retc);
};
