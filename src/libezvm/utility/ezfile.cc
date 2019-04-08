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
#include "ezvm/ezfile.h"
#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <stdexcept>

ezFile::ezFile() : m_fd(NULL) {}

ezFile::ezFile(const string path, const string mode) : m_fd(NULL) {
  if (path == "stdout") {
    m_fd = stdout;
  } else if (path == "stderr") {
    m_fd = stderr;
  } else if (!path.empty()) {
    if (mode.empty())
      throw runtime_error("mode is empty");
    m_fd = fopen(path.c_str(), mode.c_str());
    if (!m_fd)
      throw runtime_error(strerror(errno));
  }
}

ezFile::~ezFile() {
  if (!m_fd || m_fd == stdout || m_fd == stderr)
    return;
  fclose(m_fd);
}

void ezFile::println(const char *fmt, ...) {
  if (!m_fd)
    return;
  va_list ap;
  va_start(ap, fmt);
  vfprintf(m_fd, fmt, ap);
  va_end(ap);
  fprintf(m_fd, "\n");
}

void ezFile::vprintln(const char *fmt, va_list ap) {
  if (!m_fd)
    return;
  vfprintf(m_fd, fmt, ap);
  fprintf(m_fd, "\n");
}

void ezFile::print(const char *fmt, ...) {
  if (!m_fd)
    return;
  va_list ap;
  va_start(ap, fmt);
  vfprintf(m_fd, fmt, ap);
  va_end(ap);
}

void ezFile::vprint(const char *fmt, va_list ap) {
  if (!m_fd)
    return;
  vfprintf(m_fd, fmt, ap);
}
