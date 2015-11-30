#pragma once

#include "ezaddr.h"
#include "ezval.h"
#include "ezfile.h"
#include <cstddef>
#include <vector>
#include <string>
#include <map>

using namespace std;

class ezAsmProcedure {
	private:
		size_t label2index(string label);
		ezCarousel* m_carousel;
	public:
		ezAsmProcedure(ezCarousel* carousel);
		void call(const ezAddress& func, vector<ezAddress>& args, vector<ezAddress>& rets);
		void mv(vector<ezAddress>& dest, vector<ezAddress>& src);
		void ld(const ezAddress dest, const ezAddress obj, const ezAddress offset);
		void add(const ezAddress dest, vector<ezAddress>& src);
		void add(const ezAddress dest, const ezAddress cond, vector<ezAddress>& src);
		void beq(const ezAddress cond, string label);
		void bra(string label);
		void label(string name);
};

class ezASM {
	private:
		ezAddress& m_entry;
		string m_entry_string;
		vector<ezValue*>& m_constants;
		vector< vector<ezValue*>* >& m_globals;
		map<string, size_t> m_seg_symtab;
		vector< map<string, size_t>* > m_offset_symtab;
	public:
		ezASM(ezAddress& entry, vector<ezValue*>& constants, vector< vector<ezValue*>* >& globals);
		~ezASM();
		void import(const string name);
		void entry(const string name);
		ezAsmProcedure* new_proc(const string name, int argc, int retc, int mems);
		size_t global(const string value);
		size_t constant(const string value);
		size_t constant(const int value);
		size_t offset(const string segment, const string value);
		size_t segment(const string segment);
		void dump(ezFile& sink);
};
