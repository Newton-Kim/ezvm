#include "ezvm/ezvm.h"
#include "ezvm/ezlog.h"
#include "ezas.tab.cc.h"
#include <getopt.h>
#include <iostream>
#include <cstring>
#include <string>
#include <cerrno>
#include <cstdio>

using namespace std;
#define VERSION "1.0.0"

extern void ezparse(FILE* fd, const string target);

void show_help(const char* name) {
	cout << name << " version " << VERSION << endl;
	cout << "Usage: " << name << " [option]... asm_file" << endl;
	cout << "Option:" << endl;
	cout << "  -h, --help  shows this screen" << endl;
	cout << "  -v, --version  prints the version" << endl;
	cout << "  -c, --compile  compiles the asm_file to create ezf_file" << endl;
	cout << "  -l, --log  writes logs to the file or stream(stderr or stdout)" << endl;
}

int main(int argc, char* argv[]) {
	int c, option_index;
	bool help_flag = false, version_flag = false, verbose_flag = false;
	string source, target, logger;
	struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"version", no_argument, 0, 'v'},
		{"verbose", no_argument, 0, 0},
		{"compile", required_argument, 0, 'c'},
		{"log", required_argument, 0, 'l'},
		{0, 0, 0, 0}
	};
	while(1) {
		c = getopt_long(argc, argv, "hvc:l:", long_options, &option_index);
//		if(optarg) cout << "optarg:" << optarg << endl;
		if(c == -1) break;
//		cout << "c:" << (char)c << endl;
		switch(c) {
			case 0:
				if (!strcmp("verbose", long_options[option_index].name)) {
					cerr << "verbose true" << endl;
					verbose_flag = true;
				}
				break;
			case 'h':
				help_flag = true;
				break;
			case 'v':
				version_flag = true;
				break;
			case 'c':
				if(!optarg) {
					cerr << "error: compiling target is missing" << endl;
					return 0;
				}
				target = optarg;
				break;
			case 'l':
				if(!optarg) {
					cerr << "error: logging target is missing" << endl;
					return 0;
				}
				ezLog::initialize(optarg);
				break;
			default:
				cerr << "invalid argument" << (char)c << endl;
				return 1;
				break;
		}
	}
	if(help_flag) show_help(argv[0]);
	if(version_flag) cout << VERSION << endl;
	while(optind < argc) {
		if(source.empty()) source = argv[optind++]; 
		else {
			cerr << "error: multiple source, " << argv[optind] << endl;
			return 1;
		}
	}
	FILE* fd = fopen(source.c_str(), "rb");
	if(!fd) {
		cerr << "error: " << strerror(errno) << endl;
		return 1;
	}
	ezparse(fd, target);
	fclose(fd);
	return 0;
}
