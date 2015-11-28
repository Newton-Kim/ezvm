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
	thd.call(arg1, arg2);
}

void run_add(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	thd.add(arg1, arg2);
}

RUNFUNC s_run[] = {
	run_mv,
	run_ld,
	run_call,
	run_add
};


ezThread::ezThread(ezAddress entry, vector< vector<ezValue*>* >& globals, vector<ezValue*>& constants):
		m_entry(entry),
		m_constants(constants),
		m_globals(globals)
{
	ezValue* v = addr2val(entry);
	switch (v->type) {
		case EZ_VALUE_TYPE_CAROUSEL:
			{
				ezCarousel* crsl = (ezCarousel*)v;
				ezStackFrame* sf = new ezStackFrame(crsl);
				for(size_t i = 0 ; i < crsl->nmems ; i++) sf->local.push_back(ezNull::instance());
				m_stack.push(sf);
			}
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
	//It needs 3 steps to finalize a thread
	if(m_stack.empty()) return EZ_STEP_DONE;
	ezStackFrame* sf = m_stack.top();
	ezLog& log = ezLog::instance();
	if(sf->pc >= sf->carousel->instruction.size()) {
		log.verbose("stack %p has poped out", sf);
		m_stack.pop();
		size_t rets = sf->returns.size();
		size_t dests = sf->return_dest.size();
		size_t cnt = (rets > dests) ? dests : rets;
		for(size_t i = 0 ; i < cnt ; i++) val2addr(sf->return_dest[i], sf->returns[i]);
		if(dests > rets) {
			for(size_t i = cnt ; i < dests ; i++)
				val2addr(sf->return_dest[i], ezNull::instance());
		}
		delete sf;
		return EZ_STEP_CONTINUE;
	}
	log.verbose("stack %p has turn", sf);
	ezInstDecoder decoder;
	ezOpCode op;
	uint8_t arg1, arg2, arg3;
	decoder.opcode(sf->carousel->instruction[sf->pc++], op, arg1, arg2, arg3);
	s_run[op](*this, arg1, arg2, arg3);
	return EZ_STEP_CONTINUE;
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
	vector<ezValue*> q;
	for(i = 0 ; i < cnt ; i++){
		decoder.argument(sf->carousel->instruction[sf->pc + i + ndests], src_addr);
		v = addr2val(src_addr);
		q.push_back(v);
	}
	for(i = 0 ; i < cnt ; i++){
		decoder.argument(sf->carousel->instruction[sf->pc + i], dest_addr);
		v = addr2val(dest_addr);
		v = q[i];
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
		v->reference();
		nsf->local.push_back(v);
	}
	if(func->nargs > nargs) {
		for(size_t i = min_args ; i < func->nargs ; i++, sf->pc++) {
			nsf->local.push_back(ezNull::instance());
		}
	}
	for(size_t i = 0 ; i < func->nmems ; i++) nsf->local.push_back(ezNull::instance());
	vector<ezAddress> ret_dest;
	for(size_t i = 0 ; i < nrets ; i++, sf->pc++) {
		decoder.argument(sf->carousel->instruction[sf->pc], addr);
		nsf->return_dest.push_back(addr);
	}
	m_stack.push(nsf);
}

void ezThread::add(uint8_t ndests, uint8_t nsrcs) {
	ezStackFrame* sf = m_stack.top();
	ezInstDecoder decoder;
	ezAddress dest, addr;
	decoder.argument(sf->carousel->instruction[sf->pc++], dest);
	decoder.argument(sf->carousel->instruction[sf->pc++], addr);
	ezValue* v = addr2val(addr);
	ezValue* rst = v->duplicate();
	for(size_t i = 1 ; i < nsrcs ; i++) {
		decoder.argument(sf->carousel->instruction[sf->pc++], addr);
		v = addr2val(addr);
		rst->add(v);
	}
	val2addr(dest, rst);
}

ezValue* ezThread::addr2val(ezAddress addr){
	ezValue* v = NULL;
	if(addr.segment == EZ_ASM_SEGMENT_CONSTANT) {
		if(addr.offset >= m_constants.size()) throw runtime_error("constant memory access violation");
		v = m_constants[addr.offset];
	} else if(addr.segment == EZ_ASM_SEGMENT_LOCAL) {
		ezStackFrame* sf = m_stack.top();
		if(addr.offset >= sf->local.size()) throw runtime_error("local memory access violation");
		v = sf->local[addr.offset];
	} else if(addr.segment == EZ_ASM_SEGMENT_PARENT) {
		throw runtime_error("parent segment has not been implemented");
	} else if(addr.segment >= EZ_ASM_SEGMENT_GLOBAL) {
		if(addr.segment >= m_globals.size()) throw runtime_error("invalid segment");
		if(addr.offset >= m_globals[addr.segment]->size()) throw runtime_error("global memory access violation");
		v = (*m_globals[addr.segment])[addr.offset];
	} else {
		throw runtime_error("out of segment boundary");
	}
	return v;
}

void ezThread::val2addr(vector<ezAddress>& addr, vector<ezValue*>& vals) {
	size_t addr_sz = addr.size(), vals_sz = vals.size();
	size_t gcv = (addr_sz > vals_sz) ? vals_sz : addr_sz;
	for(size_t i = 0 ; i < gcv ; i++) {
		ezValue* v = vals[i];
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
		if(addr.offset >= sf->local.size()) throw runtime_error("local memory access violation");
		v->reference();
		sf->local[addr.offset]->release();
		sf->local[addr.offset] = v;
	} else if(addr.segment == EZ_ASM_SEGMENT_PARENT) {
		throw runtime_error("parent segment has not been implemented");
	} else if(addr.segment >= EZ_ASM_SEGMENT_GLOBAL) {
		if(addr.segment >= m_globals.size()) throw runtime_error("invalid segment");
		if(addr.offset >= m_globals[addr.segment]->size()) throw runtime_error("global memory access violation");
		v->reference();
		(*m_globals[addr.segment])[addr.offset]->release(); 
		(*m_globals[addr.segment])[addr.offset] = v; 
	} else {
		throw runtime_error("out of segment boundary");
	}

	return v;
}
