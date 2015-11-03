#include "ezvm/ezlog.h"
#include "ezvm/ezthread.h"

ezThread::ezThread(ezAddress entry, vector< vector<size_t>* >& globals, vector<ezValue*>& constants):
		m_entry(entry),
		m_constants(constants),
		m_globals(globals)
{
	ezLog::logger().print("entry=%lu:%lu", entry.segment, entry.offset);
	ezValue* v = NULL;
	switch(entry.segment) {
		case EZ_ASM_SEGMENT_CONSTANT:
			v = m_constants[entry.offset];
			break;
		case EZ_ASM_SEGMENT_GLOBAL:
			v = m_constants[m_globals[0]->operator[](entry.offset)];
			break;
		default:
			//TODO: make it directive
			v = m_constants[m_globals[entry.segment - 4]->operator[](entry.offset)];
			break;
	}
	ezStackFrame* sf = NULL;
	ezLog::logger().print("entry v->type is %d", v->type);
	switch (v->type) {
		case EZ_VALUE_TYPE_CAROUSEL:
			sf = new ezStackFrame((ezCarousel*)v, m_args, m_rets);
			break;
		case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
			sf = new ezStackFrame((ezNativeCarousel*)v, m_args, m_rets);
			break;
		default:
			throw ("invalid value type");
			break;
	}
	m_stack.push(sf);
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
	ezStepState status = sf->step();
	switch(status) {
		case EZ_STEP_CONTINUE:
			return status;
			break;
		case EZ_STEP_DONE:
			delete sf;
			m_stack.pop();
			break;
	}
	return (m_stack.empty()) ? EZ_STEP_DONE : EZ_STEP_CONTINUE;
}
