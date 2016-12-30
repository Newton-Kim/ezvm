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
#include "ezio.h"
#include "ezvm/ezval.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

class ezIoPrint : public ezNativeCarousel {
 public:
  ezIoPrint();
  void run(vector<ezValue*>& args, vector<ezValue*>& rets);
};

ezIoPrint::ezIoPrint() : ezNativeCarousel() {}

void ezIoPrint::run(vector<ezValue*>& args, vector<ezValue*>& rets) {
  rets.clear();
  stringstream ss;
  size_t len = args.size();
  if (args[0]->type != EZ_VALUE_TYPE_INTEGER)
    throw runtime_error("argument 1 is not a number");
  size_t ioidx = ((ezInteger*)args[0])->to_integer();
  for (size_t i = 1; i < len; i++) {
    ezValue* v = args[i];
    switch (v->type) {
      case EZ_VALUE_TYPE_INTEGER:
        ss << ((ezInteger*)v)->to_integer();
        break;
      case EZ_VALUE_TYPE_STRING:
        ss << ((ezString*)v)->to_string();
        break;
      case EZ_VALUE_TYPE_BOOL:
        ss << (((ezBool*)v)->to_bool() ? "true" : "false");
        break;
      default:
        ss << hex << (void*)v << dec;
    }
  }
  ss << endl;
  switch (ioidx) {
    case 1:
      cout << ss.str();
      break;
    case 2:
      cerr << ss.str();
      break;
    default:
      throw runtime_error("invalid IO index");
      break;
  }
}

void ezIO::load(char*** symtab, ezValue*** constants) {
  static ezIoPrint io_print;
  static const char* io_symtab[] = {"print", NULL};
  static ezValue* io_constants[] = {&io_print, NULL};
  *symtab = (char**)io_symtab;
  *constants = io_constants;
}
