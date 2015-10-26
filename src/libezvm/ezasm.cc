#include "ezvm/ezasm.h"
#include "intrinsic/ezintrinsic.h"
#include <iostream>

using namespace std;

void ezAsmProcedure::call(const ezAddress& func, vector<ezAddress>& args, vector<ezAddress>& rets){
}

void ezAsmProcedure::mv(vector<ezAddress>& dest, vector<ezAddress>& src){
}

void ezAsmProcedure::ld(const ezAddress dest, const ezAddress obj, const ezAddress offset){
}

ezASM::ezASM(ezAddress& entry, vector<ezValue*>& constants, vector< vector<ezAddress> >& globals):
	m_entry(entry),
	m_constants(constants),
	m_globals(globals) {
}

void ezASM::import(const string entry) {
	char** symtab = NULL;
	ezValue** constants = NULL;
	//TODO:load a shared object ('lib'+entry+'.so')
	{}
	//TODO:load the intrinsic library
	{}
	ezIntrinsic::load(entry, &symtab, &constants);
	if(!symtab || !constants) return;
	size_t segment = m_globals.size();
	vector<ezAddress> offsets;
	map<string, size_t> offset_symtab;
	for(size_t i = 0 ; constants[i] && symtab[i] ; i++) {
		ezValue* constant = constants[i];
		const char* symbol = symtab[i];
		size_t offset = m_constants.size();
		offsets.push_back(ezAddress(segment, offset));
		m_constants.push_back(constant);
		offset_symtab[symbol] = offset;
	}
	if(!offsets.size()) return;
	m_seg_symtab[entry] = segment;
	m_globals.push_back(offsets);
}

void ezASM::entry(const string entry) {
	m_entry_string = entry;
}

ezAsmProcedure* ezASM::new_proc(const string name, int argc, int retc) {
	return NULL;
}

size_t ezASM::offset(const size_t segment, const string value) {
}

size_t ezASM::offset(const size_t segment, const int value) {
}

size_t ezASM::offset(const string segment, const string value) {
}

size_t ezASM::segment(const string value) {
	map<string, size_t>::iterator it = m_seg_symtab.find(value);
	if(it == m_seg_symtab.end()) throw runtime_error(value + " not found");
	return it->second;
}

