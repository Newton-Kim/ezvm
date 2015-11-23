#include "ezvm/ezasm.h"
#include "ezvm/ezlog.h"
#include "intrinsic/ezintrinsic.h"
#include "ezinstruction.h"
#include <iostream>
#include <stdexcept>

using namespace std;

ezAsmProcedure::ezAsmProcedure(ezCarousel* carousel): m_carousel(carousel) { }

void ezAsmProcedure::call(const ezAddress& func, vector<ezAddress>& args, vector<ezAddress>& rets){
	ezInstEncoder instruction(m_carousel->instruction);
	instruction.opcode(EZ_OP_CALL, args.size(), rets.size());
	instruction.argument(func);
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

void ezAsmProcedure::add(const ezAddress dest, vector<ezAddress>& src){
	ezInstEncoder instruction(m_carousel->instruction);
	instruction.opcode(EZ_OP_ADD, 1, src.size());
	instruction.argument(dest);
	for(vector<ezAddress>::iterator it = src.begin() ; it != src.end() ; it++)
		instruction.argument(*it);
}

ezASM::ezASM(ezAddress& entry, vector<ezValue*>& constants, vector< vector<ezValue*>* >& globals):
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
	ezLog& log = ezLog::instance();
	char** symtab = NULL;
	ezValue** constants = NULL;
	//TODO:load a shared object ('lib'+entry+'.so')
	{}
	//TODO:load the intrinsic library
	{}
	ezIntrinsic::load(entry, &symtab, &constants);
	if(!symtab || !constants) throw runtime_error(entry + " is not found");
	size_t segment = m_globals.size();
	vector<ezValue*>* offsets = new vector<ezValue*>;
	map<string, size_t>* offset_symtab = new map<string, size_t>;
	for(size_t i = 0 ; constants[i] && symtab[i] ; i++) {
		ezValue* constant = constants[i];
		const char* symbol = symtab[i];
		size_t offset = m_constants.size();
		offsets->push_back(constant);
		ezLog::instance().debug("global[%lu][%lu] = %s", m_globals.size(), offsets->size() - 1, symbol);
		m_constants.push_back(constant);
		(*offset_symtab)[symbol] = offset;
		log.debug("constant[%lu] = %s", offset, symbol);
	}
	if(!offsets->size()) {
		log.debug("no content is found in %s", entry.c_str());
		return;
	}
	m_seg_symtab[entry] = segment;
	m_offset_symtab.push_back(offset_symtab);
	log.debug("global[%lu] = %s", m_globals.size(), entry.c_str());
	m_globals.push_back(offsets);
}

void ezASM::entry(const string entry) {
	m_entry_string = entry;
}

ezAsmProcedure* ezASM::new_proc(const string name, int argc, int retc, int mems) {
	vector<ezValue*>* offset = m_globals[0];
	map<string, size_t>* offset_symtab = m_offset_symtab[0];
	map<string, size_t>::iterator it = offset_symtab->find(name);
	if(it != offset_symtab->end()) throw runtime_error("global symbol " + name + " already exists");
	ezCarousel* carousel = new ezCarousel(argc, retc, mems);
	size_t addr = m_constants.size();
	carousel->reference();
	m_constants.push_back(carousel);
	carousel->reference();
	offset->push_back(carousel);
	(*offset_symtab)[name] = offset->size() - 1;
	if(name == m_entry_string) {
		m_entry.segment = EZ_ASM_SEGMENT_GLOBAL;
		m_entry.offset = offset->size() - 1;
	}
	return new ezAsmProcedure(carousel);
}

size_t ezASM::global(const string value) {
//	vector<ezValue*>* offset = m_globals[0];
	map<string, size_t>* offset_symtab = m_offset_symtab[0];
	map<string, size_t>::iterator it = offset_symtab->find(value);
	if(it == offset_symtab->end()) throw runtime_error("global symbol " + value + " is not found");
	return it->second;
}

size_t ezASM::constant(const string value) {
	for(size_t i = 0 ; i < m_constants.size() ; i++) {
		ezValue* v = m_constants[i];
		if(v->type == EZ_VALUE_TYPE_STRING && ((ezString*)v)->value() == value) return i;
	}
	size_t idx = m_constants.size();
	ezValue* v = new ezString(value);
	v->reference();
	m_constants.push_back(v);
	return idx;
}

size_t ezASM::constant(const int value) {
	for(size_t i = 0 ; i < m_constants.size() ; i++) {
		ezValue* v = m_constants[i];
		if(v->type == EZ_VALUE_TYPE_INTEGER && ((ezInteger*)v)->value() == value) return i;
	}
	size_t idx = m_constants.size();
	ezValue* v = new ezInteger(value);
	v->reference();
	m_constants.push_back(v);
	return idx;
}

size_t ezASM::offset(const string segment, const string value) {
	map<string, size_t>::iterator sit = m_seg_symtab.find(segment);
	if(sit == m_seg_symtab.end()) throw runtime_error("a segment of " + segment + " is not found");
	size_t seg = sit->second;
	if(! (m_globals.size() > seg)) throw runtime_error("invalid segment id for globals : " + seg);
//	vector<ezValue*>* offset = m_globals[seg];
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

void ezASM::dump(ezFile& sink) {
	sink.print(".symtab:\n");
	sink.print("  segments:\n");
	for(map<string, size_t>::iterator it = m_seg_symtab.begin() ; it != m_seg_symtab.end() ; it++)
		sink.print("    %s->%lu\n", it->first.c_str(), it->second);
	sink.print("  offsets\n");
	for(size_t i = 0 ; i < m_offset_symtab.size() ; i++){
		sink.print("    [%lu]:\n", i);
		map<string, size_t>* offset = m_offset_symtab[i];
		for(map<string, size_t>::iterator it = offset->begin() ; it != offset->end() ; it++)
			sink.print("      %s->%lu\n", it->first.c_str(), it->second);
	}
}
