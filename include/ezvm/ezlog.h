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
#include "ezfile.h"
#include <string>

using namespace std;

/**
* @brief exLog produces footprint of internal operation.
*/
class ezLog {
 private:
  /**
  * @brief is a file stream.
  */
  ezFile m_log;
  void print(const char* level, const char* fmt, va_list ap);

 public:
  /**
  * @brief is a default constructor.
  */
  ezLog();
  /**
  * @brief constructs the file stream of logsink.
  *
  * @param logsink is a path name of the file stream.
  */
  ezLog(const string logsink);
  /**
  * @brief initializes a log with the file stream of logsink.
  *
  * @param logsink is a path name of the file stream.
  */
  static void initialize(const string logsink);
  /**
  * @brief returns a log instance.
  *
  * @return A reference to the log stream which is created in initialize
  *function.
  */
  static ezLog& instance(void);
  /**
  * @brief produces a log of debug priority.
  *
  * @param fmt is a format string, which is collection of directives. See that
  *of fprintf in 'man fprintf'.
  * @param ... is variable arguments.
  */
  void debug(const char* fmt, ...);
  /**
  * @brief produces a log of info priority.
  *
  * @param fmt is a format string, which is collection of directives. See that
  *of fprintf in 'man fprintf'.
  * @param ... is variable arguments.
  */
  void info(const char* fmt, ...);
  /**
  * @brief produces a log of verbose priority.
  *
  * @param fmt is a format string, which is collection of directives. See that
  *of fprintf in 'man fprintf'.
  * @param ... is variable arguments.
  */
  void verbose(const char* fmt, ...);
};
