#include "ezvm/ezstack.h"
#include "ezvm/ezlog.h"
#include "ezinstruction.h"

ezStackFrame::ezStackFrame(ezCarousel* crsl): pc(0), carousel(crsl) {
	ezLog::logger().print("%s", __PRETTY_FUNCTION__);
}

