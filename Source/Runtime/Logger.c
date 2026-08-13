#include "Logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LogOutput(Severity_t severity, const char *msg, ...) {
  char *mssage = nullptr;
  char buffer[8000];
  va_list list;

  va_start(list, msg);
  vsprintf(buffer, msg, list);
  va_end(list);

  if (severity == LOG_SEVERITY_INFO) {
    char message[] = "[INFO] ";
    mssage = message;
  } else if (severity == LOG_SEVERITY_WARN) {
    char message[] = "[WARN] ";
    mssage = message;
  } else if (severity == LOG_SEVERITY_ERROR) {
    char message[] = "[ERROR] ";
    mssage = message;
  } else if (severity == LOG_SEVERITY_FATAL) {
    char message[] = "[FATAL] ";
    mssage = message;
  }

  strcat(mssage, buffer);
  printf("%s\n", mssage);

  if (severity == LOG_SEVERITY_FATAL) {
    exit(1);
  }
  if (severity == LOG_SEVERITY_ERROR) {
    getchar();
  }
}
