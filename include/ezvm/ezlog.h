#pragma once
#include "ezfile.h"
#include <string>

using namespace std;

class ezLog {
	private:
		ezFile m_file;
		void print(const char* level, const char* fmt, va_list ap);
	public:
		ezLog();
		ezLog(const string target);
		static void initialize(const string target);
		static ezLog& instance(void);
		void debug(const char* fmt, ...);
		void info(const char* fmt, ...);
		void verbose(const char* fmt, ...);
};
