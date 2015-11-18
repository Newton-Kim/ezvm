#include "ezvm/ezlog.h"
#include <stdexcept>

static ezLog* s_plog = NULL;
void ezLog::initialize(const string logsink, const string dumpsink) {
	if(s_plog) delete s_plog;
	s_plog = new ezLog(logsink, dumpsink);
}

ezLog& ezLog::instance(void) {
	if (!s_plog) s_plog = new ezLog("", "");
	return *s_plog;
}

ezLog::ezLog(const string logsink, const string dumpsink): m_log(logsink, "wb"), m_dump(dumpsink, "wb") {}

void ezLog::print(const char* level, const char* fmt, va_list ap){
	m_log.print(level);
	m_log.vprintln(fmt, ap);
}

void ezLog::debug(const char* fmt, ...){
	if(m_log.isnil()) return;
	va_list ap;
	va_start(ap, fmt);
	print("D:", fmt, ap);
	va_end(ap);
}

void ezLog::info(const char* fmt, ...){
	if(m_log.isnil()) return;
	va_list ap;
	va_start(ap, fmt);
	print("I:", fmt, ap);
	va_end(ap);
}

void ezLog::verbose(const char* fmt, ...){
	if(m_log.isnil()) return;
	va_list ap;
	va_start(ap, fmt);
	print("V:", fmt, ap);
	va_end(ap);
}

void ezLog::dump(const char* fmt, ...){
	if(m_dump.isnil()) return;
	va_list ap;
	va_start(ap, fmt);
	m_dump.vprintln(fmt, ap);
	va_end(ap);
}

