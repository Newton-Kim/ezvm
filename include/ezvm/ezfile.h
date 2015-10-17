#pragma once
#include <cstdio>
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
};
