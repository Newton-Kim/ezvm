#include "ezvm/ezlog.h"
#include "ezvm/ezthread.h"
#include <stdexcept>
#include "ezinstruction.h"

typedef void (*RUNFUNC)(ezThread&, uint8_t , uint8_t , uint8_t );

void run_mv(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	thd.mv(arg1, arg2, arg3);
}

void run_ld(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	thd.ld(arg1, arg2, arg3);
}

void run_call(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	thd.call(arg1, arg2, arg3);
}

RUNFUNC s_run[] = {
	run_mv,
	run_ld,
	run_call
};


ezThread::ezThread(ezAddress entry, vector< vector<size_t>* >& globals, vector<ezValue*>& constants):
		m_entry(entry),
		m_constants(constants),
		m_globals(globals)
{
	ezLog::logger().print("entry=%lu:%lu", entry.segment, entry.offset);
	ezValue* v = addr2val(entry);
	ezLog::logger().print("entry v->type is %d", v->type);
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
	decoder.opcode(sf->carousel->instruction[sf->pc], op, arg1, arg2, arg3);
	s_run[op](*this, arg1, arg2, arg3);
	if(sf->pc >= sf->carousel->instruction.size()) return EZ_STEP_DONE;
	return (m_stack.empty()) ? EZ_STEP_DONE : EZ_STEP_CONTINUE;
}

void ezThread::mv(uint8_t arg1, uint8_t arg2, uint8_t arg3){
}

void ezThread::ld(uint8_t arg1, uint8_t arg2, uint8_t arg3){
}

void ezThread::call(uint8_t arg1, uint8_t arg2, uint8_t arg3){
}

ezValue* ezThread::addr2val(ezAddress addr){
	ezValue* v = NULL;
	if(addr.segment == EZ_ASM_SEGMENT_CONSTANT) {
		v = m_constants[addr.offset];
	} else if(addr.segment == EZ_ASM_SEGMENT_LOCAL) {
		ezStackFrame* sf = m_stack.top();
		v = sf->local[addr.offset];
	} else if(addr.segment == EZ_ASM_SEGMENT_PARENT) {
		throw runtime_error("parent segment has not been implemented");
	} else if(addr.segment >= EZ_ASM_SEGMENT_GLOBAL) {
		v = m_constants[m_globals[addr.segment]->operator[](addr.offset)];
	}
	return v;
}
