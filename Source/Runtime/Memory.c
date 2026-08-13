#include "Memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MemStats {
  uint64_t TotalAllocated;
};

static struct MemStats stats;

void InitializeMemory() { ZeroMemory(&stats, sizeof(stats)); }
void ShutdownMemory() {}

void *ZeroMemory(void *chunk, uint64_t size) { return memset(chunk, 0, size); }

void *CopyMemory(void *dest, const void *source, uint64_t size) {
  return memcpy(dest, source, size);
}

void *Allocate(uint64_t size) {
  stats.TotalAllocated += size;

  void *chunk = malloc(size);
  ZeroMemory(chunk, size);
  return chunk;
}

void *ReAllocate(void *chunk, uint64_t size, uint64_t newsize) {
  stats.TotalAllocated -= size;
  stats.TotalAllocated += newsize;
  return realloc(chunk, newsize);
}

void Free(void *chunk, uint64_t size) {
  stats.TotalAllocated -= size;
  free(chunk);
}

void *SetMemory(void *dest, int32_t value, uint64_t size) {
  return memset(dest, value, size);
}

char *GetMemoryUsage() {
  char msg[64];

  double value = (double)stats.TotalAllocated;
  const char *unit = "B";

  if (value >= 1024.0) {
    value /= 1024.0;
    unit = "KiB";
  }
  if (value >= 1024.0) {
    value /= 1024.0;
    unit = "MiB";
  }
  if (value >= 1024.0) {
    value /= 1024.0;
    unit = "GiB";
  }

  snprintf(msg, sizeof(msg), "System memory in use: %.2f %s", value, unit);
  return strdup(msg);
}
