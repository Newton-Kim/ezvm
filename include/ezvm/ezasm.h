#pragma once

#include "ezaddr.h"
#include "ezval.h"
#include <cstddef>
#include <vector>
#include <string>
#include <map>

using namespace std;

class ezAsmProcedure {
	public:
		void call(const ezAddress& func, vector<ezAddress>& args, vector<ezAddress>& rets);
		void mv(vector<ezAddress>& dest, vector<ezAddress>& src);
		void ld(const ezAddress dest, const ezAddress obj, const ezAddress offset);
};

class ezASM {
	private:
		ezAddress& m_entry;
		string m_entry_string;
		vector<ezValue*>& m_constants;
		vector< vector<ezAddress> >& m_globals;
		map<string, size_t> m_seg_symtab;
		vector< map<string, size_t> > m_offset_symtab;
	public:
		ezASM(ezAddress& entry, vector<ezValue*>& constants, vector< vector<ezAddress> >& globals);
		void import(const string name);
		void entry(const string name);
		ezAsmProcedure* new_proc(const string name, int argc, int retc);
		size_t offset(const size_t segment, const string value);
		size_t offset(const size_t segment, const int value);
		size_t offset(const string segment, const string value);
		size_t segment(const string segment);
};
