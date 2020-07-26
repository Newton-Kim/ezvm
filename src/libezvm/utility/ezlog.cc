#include "ezvm/ezlog.h"

#include <cstdarg>
#include <cstdio>

class ezLogImplemented : public ezLog {
private:
  ezLogLevel m_level;

public:
  ezLogImplemented();
  ezLog *instance(void);
  void set_log_level(ezLogLevel level);
  void log(const ezLogLevel level, const char *function, const char *file,
           const int line, const char *format, ...);
};

ezLogImplemented::ezLogImplemented() : m_level(EZ_LOG_NONE) {}

ezLog *ezLog::instance(void) {
  static ezLogImplemented s_log;
  return &s_log;
}

void ezLogImplemented::set_log_level(ezLogLevel level) { m_level = level; }

void ezLogImplemented::log(const ezLogLevel level, const char *function,
                           const char *file, const int line, const char *format,
                           ...) {
  if (level < m_level)
    return;
  const char *lvl_str = "U";
  switch (level) {
  case EZ_LOG_INFO:
    lvl_str = "I";
    break;
  case EZ_LOG_DEBUG:
    lvl_str = "D";
    break;
  case EZ_LOG_WARNING:
    lvl_str = "W";
    break;
  case EZ_LOG_ERROR:
    lvl_str = "E";
    break;
  }
  fprintf(stderr, "[%s:%d] %s %s :: ", file, line, function, lvl_str);
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  fprintf(stderr, "\n");
}
