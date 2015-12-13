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
#pragma once
#include <cstdio>
#include <cstdarg>
#include <string>

using namespace std;

/**
* @brief ezFile is a wrapper of the file system for ease of use in ezVM.
*/
class ezFile {
 private:
  /**
  * @brief is a file descriptor.
  */
  FILE* m_fd;

 public:
  /**
  * @brief is a default constructor. It creates a null file object, which does
  * nothing like /dev/null.
  */
  ezFile();
  /**
  * @brief creates a file object whose name is path. The argument mode follows
  *the rule of the mode of fopen.
  *
  * @param path is a file name with directory names delimited by /.
  * @param mode is collection of mode descriptors described in the mode of
  *fopen.
  */
  ezFile(const string path, const string mode);
  /**
  * @brief is a default destructor. It closes the file which is opened in the
  * constructors.
  */
  ~ezFile();
  /**
  * @brief produces output according to a format as described fprintf.
  *
  * @param fmt is a format string, which is collection of directives. See that
  *of fprintf in 'man fprintf'.
  * @param ... is variable arguments.
  */
  void print(const char* fmt, ...);
  /**
  * @brief produces output according to a format
  *
  * @param fmt is a format string, which is collection of directives. See that
  *of fprintf in 'man fprintf'.
  * @param ap is va_list argument.
  */
  void vprint(const char* fmt, va_list ap);
  /**
  * @brief is an equivalent funciton to print, but it is followed by a line
  *separation such as '\n'.
  *
  * @param fmt is a format string, which is collection of directives. See that
  *of fprintf in 'man fprintf'.
  * @param ... is variable arguments.
  */
  void println(const char* fmt, ...);
  /**
  * @brief is an equivalent funciton to vprint, but it is followed by a line
  *separation such as '\n'.
  *
  * @param fmt is a format string, which is collection of directives. See that
  *of fprintf in 'man fprintf'.
  * @param ap is va_list argument.
  */
  void vprintln(const char* fmt, va_list ap);
  /**
  * @brief shows if the file object is nill object.
  *
  * @return is true if the file object is equivalent to /dev/null, and false,
  *otherwise.
  */
  bool isnil(void) { return (m_fd) ? false : true; }
};
