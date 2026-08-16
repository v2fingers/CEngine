#pragma once

enum severity {
  LOG_SEVERITY_INFO,
  LOG_SEVERITY_WARN,
  LOG_SEVERITY_ERROR,
  LOG_SEVERITY_FATAL
};

void logger_log(enum severity severity, const char *msg, ...);

#define INFO(msg, ...) logger_log(LOG_SEVERITY_INFO, msg, ##__VA_ARGS__);
#define WARN(msg, ...) logger_log(LOG_SEVERITY_WARN, msg, ##__VA_ARGS__);
#define ERROR(msg, ...) logger_log(LOG_SEVERITY_ERROR, msg, ##__VA_ARGS__);
#define FATAL(msg, ...) logger_log(LOG_SEVERITY_FATAL, msg, ##__VA_ARGS__);
