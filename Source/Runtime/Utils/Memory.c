#include "Memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mem_stats {
  uint64_t TotalAllocated;
};

static struct mem_stats stats;

void memory_init() { memory_zero(&stats, sizeof(stats)); }
void memory_shutdown() {}

void *memory_zero(void *chunk, uint64_t size) { return memset(chunk, 0, size); }

void *memory_copy(void *dest, const void *source, uint64_t size) {
  return memcpy(dest, source, size);
}

void *memory_allocate(uint64_t size) {
  stats.TotalAllocated += size;

  void *chunk = malloc(size);
  memory_zero(chunk, size);
  return chunk;
}

void *memory_reallocate(void *chunk, uint64_t size, uint64_t newsize) {
  stats.TotalAllocated -= size;
  stats.TotalAllocated += newsize;
  return realloc(chunk, newsize);
}

void memory_free(void *chunk, uint64_t size) {
  stats.TotalAllocated -= size;
  free(chunk);
}

void *memory_set(void *dest, int32_t value, uint64_t size) {
  return memset(dest, value, size);
}

char *memory_getusage() {
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
