#include "ezvm/ezlog.h"
#include <stdexcept>

static ezLog* s_plog = NULL;
void ezLog::initialize(const string target) {
	if(s_plog) delete s_plog;
	s_plog = new ezLog(target);
}

ezLog& ezLog::instance(void) {
	if (!s_plog) s_plog = new ezLog();
	return *s_plog;
}

ezLog::ezLog() {}

ezLog::ezLog(const string target): m_file(target, "wb") {}

void ezLog::print(const char* level, const char* fmt, va_list ap){
	m_file.print(level);
	m_file.vprintln(fmt, ap);
}

void ezLog::debug(const char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	print("D:", fmt, ap);
	va_end(ap);
}

void ezLog::info(const char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	print("I:", fmt, ap);
	va_end(ap);
}

void ezLog::verbose(const char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	print("V:", fmt, ap);
	va_end(ap);
}

