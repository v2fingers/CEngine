#pragma once

typedef enum Severity {
  LOG_SEVERITY_INFO,
  LOG_SEVERITY_WARN,
  LOG_SEVERITY_ERROR,
  LOG_SEVERITY_FATAL
} Severity_t;

void LogOutput(Severity_t severity, const char *msg, ...);

#define INFO(msg, ...) LogOutput(LOG_SEVERITY_INFO, msg, ##__VA_ARGS__);
#define WARN(msg, ...) LogOutput(LOG_SEVERITY_WARN, msg, ##__VA_ARGS__);
#define ERROR(msg, ...) LogOutput(LOG_SEVERITY_ERROR, msg, ##__VA_ARGS__);
#define FATAL(msg, ...) LogOutput(LOG_SEVERITY_FATAL, msg, ##__VA_ARGS__);
