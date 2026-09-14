#include "Memory.h"
#include "Core/Logger.h"

#include <stdlib.h>
#include <string.h>

struct Memory_Stats {
  u64 total_allocated;
};

static struct Memory_Stats stats;

void mem_init(void) {
  mem_zero(&stats, sizeof(stats));
}
void mem_shutdown(void) {
}

void mem_free(void *chunk, u64 size) {
  LOG_INFO("Memory Free:  %p (%lu bytes)", chunk, size);
  stats.total_allocated -= size;
  free(chunk);
}

void *mem_alloc(u64 size) {
  void *chunk = malloc(size);
  if (!chunk) {
    return NULL;
  }

  stats.total_allocated += size;
  LOG_INFO("Memory Allocated: %p (%lu bytes)", chunk, size);

  mem_zero(chunk, size);
  return chunk;
}

void *mem_copy(void *dest, const void *source, u64 size) {
  return memcpy(dest, source, size);
}

void *mem_zero(void *chunk, u64 size) {
  return memset(chunk, 0, size);
}

void *mem_realloc(void *chunk, u64 size, u64 newsize) {
  stats.total_allocated -= size;
  stats.total_allocated += newsize;
  return realloc(chunk, newsize);
}

void *mem_set(void *dest, i32 value, u64 size) {
  return memset(dest, value, size);
}

void mem_print_usage(void) {
  f64 value = (double)stats.total_allocated;
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

  LOG_INFO("System memory in use: %.2f %s", value, unit);
}
