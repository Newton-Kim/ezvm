#pragma once
#include <cstdio>
#include <cstdarg>
#include <string>

using namespace std;

class ezFile {
	private:
		FILE* m_fd;
	public:
		ezFile();
		ezFile(const string path, const string mode);
		~ezFile();
		void print(const char* fmt, ...);
		void vprint(const char* fmt, va_list ap);
		void println(const char* fmt, ...);
		void vprintln(const char* fmt, va_list ap);
};
