#pragma once
#include "ezfile.h"
#include <string>

using namespace std;

class ezLog {
	private:
		ezFile m_log;
		void print(const char* level, const char* fmt, va_list ap);
	public:
		ezLog();
		ezLog(const string logsink);
		static void initialize(const string logsink);
		static ezLog& instance(void);
		void debug(const char* fmt, ...);
		void info(const char* fmt, ...);
		void verbose(const char* fmt, ...);
};
