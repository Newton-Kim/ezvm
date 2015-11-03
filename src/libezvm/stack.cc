#include "ezvm/ezstack.h"
#include "ezvm/ezlog.h"

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
	} 
	return EZ_STEP_DONE;
}
