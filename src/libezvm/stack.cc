#include "ezvm/ezstack.h"
#include "ezvm/ezlog.h"
#include "ezinstruction.h"

typedef void (*RUNFUNC)(ezStackFrame&, uint8_t , uint8_t , uint8_t );

void run_mv(ezStackFrame& sf, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	sf.mv(arg1, arg2, arg3);
}

void run_ld(ezStackFrame& sf, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	sf.ld(arg1, arg2, arg3);
}

void run_call(ezStackFrame& sf, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
	sf.call(arg1, arg2, arg3);
}

RUNFUNC s_run[] = {
	run_mv,
	run_ld,
	run_call
};

ezStackFrame::ezStackFrame(ezCarousel* carousel, vector<ezValue*>& args, vector<ezValue*>& rets): m_pc(0), m_carousel(carousel), m_native_carousel(NULL), m_args(args), m_rets(rets) {
	ezLog::logger().print("%s", __PRETTY_FUNCTION__);
}

ezStackFrame::ezStackFrame(ezNativeCarousel* carousel, vector<ezValue*>& args, vector<ezValue*>& rets): m_pc(0), m_carousel(NULL), m_native_carousel(carousel), m_args(args), m_rets(rets) {
	ezLog::logger().print("%s", __PRETTY_FUNCTION__);
}

ezStepState ezStackFrame::step(void) {
	ezLog::logger().print("m_native_carousel:%p", m_native_carousel);
	if(m_native_carousel) {
		m_native_carousel->run(m_args, m_rets);
		return EZ_STEP_DONE;
	} 
	ezInstDecoder decoder;
	ezOpCode op;
	uint8_t arg1, arg2, arg3;
	decoder.opcode(m_carousel->instruction[m_pc++], op, arg1, arg2, arg3);
	s_run[op](*this, arg1, arg2, arg3);
	return EZ_STEP_DONE;
}

void ezStackFrame::mv(uint8_t arg1, uint8_t arg2, uint8_t arg3){
}

void ezStackFrame::ld(uint8_t arg1, uint8_t arg2, uint8_t arg3){
}

void ezStackFrame::call(uint8_t arg1, uint8_t arg2, uint8_t arg3){
}

