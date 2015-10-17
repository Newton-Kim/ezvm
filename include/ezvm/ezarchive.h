#pragma once
#include <string>

using namespace std;

class ezArchive {
	public:
		void save(const string path);
		void load(const string path);
};
