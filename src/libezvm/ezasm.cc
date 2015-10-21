#include "ezvm/ezasm.h"
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
}

