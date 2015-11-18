#pragma once
#include "ezfile.h"
#include <string>

using namespace std;

class ezLog {
	private:
		ezFile m_log;
		ezFile m_dump;
		void print(const char* level, const char* fmt, va_list ap);
	public:
		ezLog(const string logsink, const string dumpsink);
		static void initialize(const string logsink, const string dumpsink);
		static ezLog& instance(void);
		void debug(const char* fmt, ...);
		void info(const char* fmt, ...);
		void verbose(const char* fmt, ...);
		void dump(const char* fmt, ...);
};
