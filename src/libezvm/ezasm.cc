#include "ezvm/ezasm.h"
#include "ezvm/ezlog.h"
#include "intrinsic/ezintrinsic.h"
#include "ezinstruction.h"
#include <iostream>
#include <stdexcept>

using namespace std;

ezAsmProcedure::ezAsmProcedure(const ezCarousel* carousel): m_carousel(carousel) {}

void ezAsmProcedure::call(const ezAddress& func, vector<ezAddress>& args, vector<ezAddress>& rets){
	ezInstEncoder instruction(m_carousel->instruction);
	instruction.opcode(EZ_OP_CALL, args.size(), rets.size());
	for(vector<ezAddress>::iterator it = args.begin() ; it != args.end() ; it++)
		instruction.argument(*it);
	for(vector<ezAddress>::iterator it = rets.begin() ; it != rets.end() ; it++)
		instruction.argument(*it);
}

void ezAsmProcedure::mv(vector<ezAddress>& dest, vector<ezAddress>& src){
	ezInstEncoder instruction(m_carousel->instruction);
	instruction.opcode(EZ_OP_MV, dest.size(), src.size());
	for(vector<ezAddress>::iterator it = dest.begin() ; it != dest.end() ; it++)
		instruction.argument(*it);
	for(vector<ezAddress>::iterator it = src.begin() ; it != src.end() ; it++)
		instruction.argument(*it);
}

void ezAsmProcedure::ld(const ezAddress dest, const ezAddress obj, const ezAddress offset){
	ezInstEncoder instruction(m_carousel->instruction);
	instruction.opcode(EZ_OP_LD, 1, 1, 1);
	instruction.argument(dest);
	instruction.argument(obj);
	instruction.argument(offset);
}

ezASM::ezASM(ezAddress& entry, vector<ezValue*>& constants, vector< vector<size_t>* >& globals):
		m_entry(entry),
		m_constants(constants),
		m_globals(globals) {
	m_offset_symtab.push_back(new map<string, size_t>);
}

ezASM::~ezASM() {
	for(vector< map<string, size_t>* >::iterator it = m_offset_symtab.begin() ; it != m_offset_symtab.end() ; it++)
		if(*it) delete *it;
}

void ezASM::import(const string entry) {
	char** symtab = NULL;
	ezValue** constants = NULL;
	//TODO:load a shared object ('lib'+entry+'.so')
	{}
	//TODO:load the intrinsic library
	{}
	ezIntrinsic::load(entry, &symtab, &constants);
	if(!symtab || !constants) throw runtime_error(entry + " is not found");
	size_t segment = m_globals.size();
	vector<size_t>* offsets = new vector<size_t>;
	map<string, size_t>* offset_symtab = new map<string, size_t>;
	for(size_t i = 0 ; constants[i] && symtab[i] ; i++) {
		ezValue* constant = constants[i];
		const char* symbol = symtab[i];
		size_t offset = m_constants.size();
		offsets->push_back(offset);
		m_constants.push_back(constant);
		(*offset_symtab)[symbol] = offset;
	}
	if(!offsets->size()) {
		ezLog::logger().print("no content is found in %s", entry.c_str());
		return;
	}
	m_seg_symtab[entry] = segment;
	m_offset_symtab.push_back(offset_symtab);
	m_globals.push_back(offsets);
}

void ezASM::entry(const string entry) {
	m_entry_string = entry;
}

ezAsmProcedure* ezASM::new_proc(const string name, int argc, int retc) {
	vector<size_t>* offset = m_globals[0];
	map<string, size_t>* offset_symtab = m_offset_symtab[0];
	map<string, size_t>::iterator it = offset_symtab->find(name);
	if(it != offset_symtab->end()) throw runtime_error("global symbol " + name + " already exists");
	ezCarousel* carousel = new ezCarousel();
	size_t addr = m_constants.size();
	m_constants.push_back(carousel);
	offset->push_back(addr);
	(*offset_symtab)[name] = addr;
	return new ezAsmProcedure(carousel);
}

size_t ezASM::global(const string value) {
	vector<size_t>* offset = m_globals[0];
	map<string, size_t>* offset_symtab = m_offset_symtab[0];
	map<string, size_t>::iterator it = offset_symtab->find(value);
	if(it == offset_symtab->end()) throw runtime_error("global symbol " + value + " is not found");
	return (*offset)[it->second];
}

size_t ezASM::constant(const string value) {
	for(size_t i = 0 ; i < m_constants.size() ; i++) {
		ezValue* v = m_constants[i];
		if(v->type == EZ_VALUE_TYPE_STRING && ((ezString*)v)->value() == value) return i;
	}
	size_t idx = m_constants.size();
	m_constants.push_back(new ezString(value));
	return idx;
}

size_t ezASM::constant(const int value) {
	for(size_t i = 0 ; i < m_constants.size() ; i++) {
		ezValue* v = m_constants[i];
		if(v->type == EZ_VALUE_TYPE_INTEGER && ((ezInteger*)v)->value() == value) return i;
	}
	size_t idx = m_constants.size();
	m_constants.push_back(new ezInteger(value));
	return idx;
}

size_t ezASM::offset(const string segment, const string value) {
	ezLog::logger().print("%s (%s, %s)", __PRETTY_FUNCTION__, segment.c_str(), value.c_str());
	map<string, size_t>::iterator sit = m_seg_symtab.find(segment);
	if(sit == m_seg_symtab.end()) throw runtime_error("a segment of " + segment + " is not found");
	size_t seg = sit->second;
	if(! (m_globals.size() > seg)) throw runtime_error("invalid segment id for globals : " + seg);
	vector<size_t>* offset = m_globals[seg];
	if(! (m_offset_symtab.size() > seg)) throw runtime_error("invalid segment id for offset table : " + seg);
	map<string, size_t>* offset_symtab = m_offset_symtab[seg];
	map<string, size_t>::iterator it = offset_symtab->find(value);
	if(it == offset_symtab->end()) throw runtime_error("an offset of " + segment + ":" + value + " is not found");
	return it->second;
}

size_t ezASM::segment(const string value) {
	map<string, size_t>::iterator it = m_seg_symtab.find(value);
	if(it == m_seg_symtab.end()) throw runtime_error(value + " not found");
	return it->second;
}

