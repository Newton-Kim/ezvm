#include "ezvm/ezstack.h"

ezStackFrame::ezStackFrame(ezCarousel* carousel, vector<ezValue*>& args, vector<ezValue*>& rets): m_pc(0), m_carousel(carousel), m_native_carould(NULL), m_args(args), m_rets(rets) {}

ezStackFrame::ezStackFrame(ezNativeCarousel* carousel, vector<ezValue*>& args, vector<ezValue*>& rets): m_pc(0), m_carousel(NULL), m_native_carould(carousel), m_args(args), m_rets(rets) {}

ezStepState ezStackFrame::step(void) {
	if(m_native_carould) {
		m_native_carould->run(m_args, m_rets);
	}
	EZ_STEP_DONE;
}
