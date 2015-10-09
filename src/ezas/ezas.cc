#include <getopt.h>
#include <iostream>

using namespace std;
#define VERSION "1.0.0"

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
	int help_flag = 0, version_flag = 0;
	struct option long_options[] = {
		{"help", no_argument, &help_flag, 'h'},
		{"version", no_argument, &version_flag, 'v'},
		{"compile", required_argument, 0, 'c'},
		{"log", required_argument, 0, 'l'},
		{0, 0, 0, 0}
	};
	while(1) {
		c = getopt_long(argc, argv, "hvc:l:", long_options, &option_index);
		if(optarg) cout << "optarg:" << optarg << endl;
		cout << "c:" << (char)c << endl;
		if(c == -1) break;
		switch(c) {
			case 'h':
			case 'v':
				break;
			case 'c':
				break;
			case 'l':
				break;
			default:
				cerr << "Invalid arguments" << endl;
				return 1;
				break;
		}
	}
	if(help_flag) {
		show_help(argv[0]);
		return 0;
	} else if(version_flag) {
		cout << VERSION << endl;
		return 0;
	}
	return 0;
}
