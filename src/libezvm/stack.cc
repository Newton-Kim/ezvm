#include "ezvm/ezstack.h"
#include "ezvm/ezlog.h"
#include "ezinstruction.h"

ezStackFrame::ezStackFrame(ezCarousel* crsl): pc(0), carousel(crsl) {
	ezLog::instance().verbose("%s", __PRETTY_FUNCTION__);
}

ezStackFrame::~ezStackFrame() {
	for(vector<ezValue*>::iterator it = local.begin() ; it != local.end() ; it++) {
		ezValue* v = *it;
		size_t ref = v->release();
		if(!ref) delete v;
	}
}
