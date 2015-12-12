/* Copyright (C) 2015 Newton Kim
* 
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditiong:
* 
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
*/
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

extern void ezparse(FILE* fd, const string target, const string dump);

void show_help(const char* name) {
  cout << name << " version " << VERSION << endl;
  cout << "Usage: " << name << " [option]... asm_file" << endl;
  cout << "Option:" << endl;
  cout << "  -h, --help  shows this screen" << endl;
  cout << "  -v, --version  prints the version" << endl;
  cout << "  -c, --compile  compiles the asm_file to create ezf_file" << endl;
  cout << "  -l, --log  writes logs to a file or stream(stderr or stdout)"
       << endl;
  cout << "  -d, --dump  dumps state to a file or stream(stderr or stdout)"
       << endl;
}

int main(int argc, char* argv[]) {
  int c, option_index;
  bool help_flag = false, version_flag = false, verbose_flag = false;
  string source, target, logger;
  string dumpsink;
  struct option long_options[] = {
    { "help", no_argument, 0, 'h' }, { "version", no_argument, 0, 'v' },
    { "verbose", no_argument, 0, 0 }, { "compile", required_argument, 0, 'c' },
    { "log", required_argument, 0, 'l' }, { "dump", required_argument, 0, 'd' },
    { 0, 0, 0, 0 }
  };
  while (1) {
    c = getopt_long(argc, argv, "hvc:l:d:", long_options, &option_index);
    //		if(optarg) cout << "optarg:" << optarg << endl;
    if (c == -1) break;
    //		cout << "c:" << (char)c << endl;
    switch (c) {
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
        if (!optarg) {
          cerr << "error: compiling target is missing" << endl;
          return 0;
        }
        target = optarg;
        break;
      case 'l':
        if (!optarg) {
          cerr << "error: logging target is missing" << endl;
          return 0;
        }
        ezLog::initialize(optarg);
        break;
      case 'd':
        if (!optarg) {
          cerr << "error: logging target is missing" << endl;
          return 0;
        }
        dumpsink = optarg;
        break;
      default:
        cerr << "invalid argument" << (char) c << endl;
        return 1;
        break;
    }
  }
  if (help_flag) show_help(argv[0]);
  if (version_flag) cout << VERSION << endl;
  while (optind < argc) {
    if (source.empty())
      source = argv[optind++];
    else {
      cerr << "error: multiple source, " << argv[optind] << endl;
      return 1;
    }
  }
  FILE* fd = fopen(source.c_str(), "rb");
  if (!fd) {
    cerr << "error: " << strerror(errno) << endl;
    return 1;
  }
  ezparse(fd, target, dumpsink);
  fclose(fd);
  return 0;
}
