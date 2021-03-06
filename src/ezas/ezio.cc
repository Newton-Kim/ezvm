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
#include "eaval.h"
#include "ezvm/ezfunc.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

class ezIoPrint : public ezUserDefinedFunction {
private:
  ostream &m_io;

public:
  ezIoPrint(ostream &io);
  ezObject *run(vector<ezObject *> &args);
};

ezIoPrint::ezIoPrint(ostream &io) : ezUserDefinedFunction(), m_io(io) {}

ezObject *ezIoPrint::run(vector<ezObject *> &args) {
  stringstream ss;
  size_t len = args.size();
  for (size_t i = 0; i < len; i++) {
    // TODO:typecheck
    ezValue *v = (ezValue *)args[i];
    switch (v->id) {
    case EZ_VALUE_TYPE_INTEGER:
      ss << ((ezInteger *)v)->value;
      break;
    case EZ_VALUE_TYPE_FLOAT:
      ss << ((ezFloat *)v)->value;
      break;
    case EZ_VALUE_TYPE_COMPLEX: {
      complex<double> c = ((ezComplex *)v)->value;
      ss << c.real();
      if (c.imag() > 0)
        ss << "+";
      ss << c.imag() << "j";
    } break;
    case EZ_VALUE_TYPE_STRING:
      ss << ((ezString *)v)->value;
      break;
    case EZ_VALUE_TYPE_BOOL:
      ss << (((ezBool *)v)->value ? "true" : "false");
      break;
    default:
      ss << hex << (void *)v << dec;
    }
  }
  ss << endl;
  m_io << ss.str();
  return NULL;
}

void ezIO::load(char ***symtab, ezObject ***constants) {
  ezIoPrint *io_stdout = new ezIoPrint(cout), *io_stderr = new ezIoPrint(cerr);
  static const char *io_symtab[] = {"stdout", "stderr", NULL};
  static ezObject *io_constants[] = {io_stdout, io_stderr, NULL};
  *symtab = (char **)io_symtab;
  *constants = io_constants;
}
