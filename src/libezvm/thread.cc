#include "ezvm/ezlog.h"
#include "ezvm/ezthread.h"
#include "ezinstruction.h"
#include <stdexcept>

typedef void (*RUNFUNC)(ezThread&, uint8_t , uint8_t , uint8_t );

void run_mv(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	thd.mv(arg1, arg2);
}

void run_ld(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	thd.ld();
}

void run_call(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	ezLog::instance().verbose("call (%d %d %d)", arg1, arg2, arg3);
	thd.call(arg1, arg2);
}

RUNFUNC s_run[] = {
	run_mv,
	run_ld,
	run_call
};


ezThread::ezThread(ezAddress entry, vector< vector<ezValue*>* >& globals, vector<ezValue*>& constants):
		m_entry(entry),
		m_constants(constants),
		m_globals(globals)
{
	ezLog::instance().verbose("entry=%d:%lu", entry.segment, entry.offset);
	ezValue* v = addr2val(entry);
	ezLog::instance().debug("entry v->type is %d", v->type);
	switch (v->type) {
		case EZ_VALUE_TYPE_CAROUSEL:
			m_stack.push(new ezStackFrame((ezCarousel*)v));
			break;
		case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
			((ezNativeCarousel*)v)->run(m_args, m_rets);
			break;
		default:
			throw ("invalid value type");
			break;
	}
}

ezThread::~ezThread() {
	while(!m_stack.empty()) {
		ezStackFrame* sf = m_stack.top();
		if(sf) delete sf;
		m_stack.pop();
	}
}

ezStepState ezThread::step(void) {
	if(m_stack.empty()) return EZ_STEP_DONE;
	ezStackFrame* sf = m_stack.top();
	ezInstDecoder decoder;
	ezOpCode op;
	uint8_t arg1, arg2, arg3;
	decoder.opcode(sf->carousel->instruction[sf->pc++], op, arg1, arg2, arg3);
	s_run[op](*this, arg1, arg2, arg3);
	if(sf->pc >= sf->carousel->instruction.size()) return EZ_STEP_DONE;
	return (m_stack.empty()) ? EZ_STEP_DONE : EZ_STEP_CONTINUE;
}

void ezThread::mv(uint8_t ndests, uint8_t nsrcs){
	//TODO it can be done via a macro
	if(m_stack.empty()) throw runtime_error("stack underrun");
	ezStackFrame* sf = m_stack.top();
	size_t cnt = (ndests > nsrcs) ? nsrcs : ndests;
	ezInstDecoder decoder;
	ezAddress src_addr;
	ezAddress dest_addr;
	ezValue* v = NULL;
	size_t i = 0;
	for(i = 0 ; i < cnt ; i++){
		decoder.argument(sf->carousel->instruction[sf->pc + i], dest_addr);
		decoder.argument(sf->carousel->instruction[sf->pc + i + ndests], src_addr);
		v = addr2val(src_addr);
		v->reference();
		val2addr(dest_addr, v);
	}
	if(ndests > nsrcs) {
		for(i = cnt ; i < ndests ; i++) {
			decoder.argument(sf->carousel->instruction[sf->pc + cnt + i], dest_addr);
			val2addr(dest_addr, ezNull::instance());
		}
	}
	sf->pc += (ndests + nsrcs);
}

void ezThread::ld(void){
	//TODO it can be done via a macro
	if(m_stack.empty()) throw runtime_error("stack underrun");
	ezStackFrame* sf = m_stack.top();
	sf->pc += 3;
}

void ezThread::call(uint8_t nargs, uint8_t nrets){
	//TODO it can be done via a macro
	if(m_stack.empty()) throw runtime_error("stack underrun");
	ezStackFrame* sf = m_stack.top();
	ezInstDecoder decoder;
	ezAddress addr;
	decoder.argument(sf->carousel->instruction[sf->pc++], addr);
	ezValue* func = addr2val(addr);
	switch(func->type) {
		case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
			call((ezNativeCarousel*)func, nargs, nrets);
			break;
		case EZ_VALUE_TYPE_CAROUSEL:
			call((ezCarousel*)func, nargs, nrets);
			break;
		default:
			throw runtime_error("function is not executable");
			break;
	}
}

void ezThread::call(ezNativeCarousel* func, uint8_t nargs, uint8_t nrets){
	ezStackFrame* sf = m_stack.top();
	ezInstDecoder decoder;
	vector<ezValue*> args;
	ezAddress addr;
	for(size_t i = 0 ; i < nargs ; i++, sf->pc++) {
		decoder.argument(sf->carousel->instruction[sf->pc], addr);
		ezValue* v = addr2val(addr);
		args.push_back(v);
	}
	vector<ezAddress> ret_dest;
	for(size_t i = 0 ; i < nrets ; i++, sf->pc++) {
		decoder.argument(sf->carousel->instruction[sf->pc], addr);
		ret_dest.push_back(addr);
	}
	vector<ezValue*> rets;
	func->run(args, rets);
	if(rets.size()) val2addr(ret_dest, rets);
}

void ezThread::call(ezCarousel* func, uint8_t nargs, uint8_t nrets){
	ezStackFrame* nsf = new ezStackFrame(func);
	ezStackFrame* sf = m_stack.top();
	ezInstDecoder decoder;
	vector<ezValue*> args;
	ezAddress addr;
	size_t min_args = (func->nargs > nargs) ? nargs : func->nargs;
	for(size_t i = 0 ; i < min_args ; i++, sf->pc++) {
		decoder.argument(sf->carousel->instruction[sf->pc], addr);
		ezValue* v = addr2val(addr);
		nsf->local.push_back(v);
	}
	if(func->nargs > nargs) {
		for(size_t i = min_args ; i < func->nargs ; i++, sf->pc++) {
			nsf->local.push_back(ezNull::instance());
		}
	}
	vector<ezAddress> ret_dest;
	for(size_t i = 0 ; i < nrets ; i++, sf->pc++) {
		decoder.argument(sf->carousel->instruction[sf->pc], addr);
		nsf->return_dest.push_back(addr);
	}
	m_stack.push(nsf);
}

ezValue* ezThread::addr2val(ezAddress addr){
	ezValue* v = NULL;
	if(addr.segment == EZ_ASM_SEGMENT_CONSTANT) {
		if(addr.offset >= m_constants.size()) throw runtime_error("memory access violation");
		v = m_constants[addr.offset];
	} else if(addr.segment == EZ_ASM_SEGMENT_LOCAL) {
		ezStackFrame* sf = m_stack.top();
		if(addr.offset >= sf->local.size()) throw runtime_error("memory access violation");
		v = sf->local[addr.offset];
	} else if(addr.segment == EZ_ASM_SEGMENT_PARENT) {
		throw runtime_error("parent segment has not been implemented");
	} else if(addr.segment >= EZ_ASM_SEGMENT_GLOBAL) {
		if(addr.offset >= m_globals[addr.segment]->size()) throw runtime_error("memory access violation");
		v = (*m_globals[addr.segment])[addr.offset];
	}
	return v;
}

void ezThread::val2addr(vector<ezAddress>& addr, vector<ezValue*>& vals) {
	size_t addr_sz = addr.size(), vals_sz = vals.size();
	size_t gcv = (addr_sz > vals_sz) ? vals_sz : addr_sz;
	for(size_t i = 0 ; i < gcv ; i++) {
		ezValue* v = vals[i];
		v->reference();
		val2addr(addr[i], v);
	}
	if(addr_sz > vals_sz) {
		for(size_t i = gcv ; i < addr_sz ; i++) val2addr(addr[i], ezNull::instance());
	}
}

ezValue* ezThread::val2addr(ezAddress addr, ezValue* v){
	if(addr.segment == EZ_ASM_SEGMENT_CONSTANT) {
		throw runtime_error("cannot write to constant");
	} else if(addr.segment == EZ_ASM_SEGMENT_LOCAL) {
		ezStackFrame* sf = m_stack.top();
		if(addr.offset >= sf->local.size()) throw runtime_error("memory access violation");
		sf->local[addr.offset] = v;
	} else if(addr.segment == EZ_ASM_SEGMENT_PARENT) {
		throw runtime_error("parent segment has not been implemented");
	} else if(addr.segment >= EZ_ASM_SEGMENT_GLOBAL) {
		if(addr.offset >= m_globals[addr.segment]->size()) throw runtime_error("memory access violation");
		(*m_globals[addr.segment])[addr.offset] = v; 
	}
	return v;
}
