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
#include "ezvm/ezlog.h"
#include <stdexcept>

static ezLog* s_plog = NULL;
void ezLog::initialize(const string logsink) {
  if (s_plog) delete s_plog;
  s_plog = new ezLog(logsink);
}

ezLog& ezLog::instance(void) {
  if (!s_plog) s_plog = new ezLog();
  return *s_plog;
}

ezLog::ezLog() {}
ezLog::ezLog(const string logsink) : m_log(logsink, "wb") {}

void ezLog::print(const char* level, const char* fmt, va_list ap) {
  m_log.print(level);
  m_log.vprintln(fmt, ap);
}

void ezLog::debug(const char* fmt, ...) {
  if (m_log.isnil()) return;
  va_list ap;
  va_start(ap, fmt);
  print("D:", fmt, ap);
  va_end(ap);
}

void ezLog::info(const char* fmt, ...) {
  if (m_log.isnil()) return;
  va_list ap;
  va_start(ap, fmt);
  print("I:", fmt, ap);
  va_end(ap);
}

void ezLog::verbose(const char* fmt, ...) {
  if (m_log.isnil()) return;
  va_list ap;
  va_start(ap, fmt);
  print("V:", fmt, ap);
  va_end(ap);
}
