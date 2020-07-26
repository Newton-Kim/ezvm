#pragma once

enum ezLogLevel {
  EZ_LOG_ALL,
  EZ_LOG_INFO,
  EZ_LOG_DEBUG,
  EZ_LOG_WARNING,
  EZ_LOG_ERROR,
  EZ_LOG_NONE
};

#ifdef EZ_LOG_ENABLED
#define EZ_INFO(format...)                                                     \
  ezLog::instance()->log(EZ_LOG_INFO, __PRETTY_FUNCTION__, __FILE__, __LINE__, \
                         format)
#define EZ_DEBUG(format...)                                                    \
  ezLog::instance()->log(EZ_LOG_DEBUG, __PRETTY_FUNCTION__, __FILE__,          \
                         __LINE__, format)
#define EZ_WARN(format...)                                                     \
  ezLog::instance()->log(EZ_LOG_WARNING, __PRETTY_FUNCTION__, __FILE__,        \
                         __LINE__, format)
#define EZ_ERROR(format...)                                                    \
  ezLog::instance()->log(EZ_LOG_ERROR, __PRETTY_FUNCTION__, __FILE__,          \
                         __LINE__, format)
#else
#define EZ_INFO
#define EZ_WARN
#define EZ_DEBUG
#define EZ_ERROR
#endif

class ezLog {
public:
  static ezLog *instance(void);
  virtual void set_log_level(ezLogLevel level) = 0;
  virtual void log(const ezLogLevel level, const char *function,
                   const char *file, const int line, const char *format,
                   ...) = 0;
};
