#include "ezvm/ezlog.h"
#include <stdexcept>

static ezFile* s_pfile = NULL;
void ezLog::initialize(const string target) {
	if(s_pfile) delete s_pfile;
	s_pfile = new ezFile(target, "wb");
}

ezFile& ezLog::logger(void) {
	if (!s_pfile) s_pfile = new ezFile();
	return *s_pfile;
}
