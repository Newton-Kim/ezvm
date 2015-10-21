#pragma once

#include "ezaddr.h"
#include <cstddef>
#include <vector>
#include <string>

using namespace std;

class ezAsmProcedure {
	public:
		void call(const ezAddress& func, vector<ezAddress>& args, vector<ezAddress>& rets);
		void mv(vector<ezAddress>& dest, vector<ezAddress>& src);
		void ld(const ezAddress dest, const ezAddress obj, const ezAddress offset);
};

class ezASM {
	public:
		void import(const string name);
		void entry(const string name);
		ezAsmProcedure* new_proc(const string name, int argc, int retc);
		size_t offset(const size_t segment, const string value);
		size_t offset(const size_t segment, const int value);
		size_t offset(const string segment, const string value);
		size_t segment(const string segment);
};
