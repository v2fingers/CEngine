#pragma once
#include "Defines.h"

enum Severity { LOG_SEVERITY_INFO, LOG_SEVERITY_WARN, LOG_SEVERITY_ERROR, LOG_SEVERITY_FATAL };

void logger_log(enum Severity severity, const char *msg, ...);
void assert_failure(const char *expression, const char *message, const char *file, i32 line);

#define ASSERT(expr, message)                                                                                               \
  do {                                                                                                                      \
    if (!(expr)) {                                                                                                          \
      assert_failure(#expr, message, __FILE__, __LINE__);                                                                   \
    }                                                                                                                       \
  } while (0)

#define LOG_INFO(msg, ...) logger_log(LOG_SEVERITY_INFO, msg, ##__VA_ARGS__);
#define LOG_WARN(msg, ...) logger_log(LOG_SEVERITY_WARN, msg, ##__VA_ARGS__);
#define LOG_ERROR(msg, ...) logger_log(LOG_SEVERITY_ERROR, msg, ##__VA_ARGS__);
#define LOG_FATAL(msg, ...) logger_log(LOG_SEVERITY_FATAL, msg, ##__VA_ARGS__);
