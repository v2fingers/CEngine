#pragma once
#include <stdint.h>

void memory_init();
void memory_shutdown();

void *memory_zero(void *chunk, uint64_t size);
void *memory_copy(void *dest, const void *source, uint64_t size);

void *memory_allocate(uint64_t size);
void *memory_reallocate(void *chunk, uint64_t size, uint64_t newsize);
void memory_free(void *chunk, uint64_t size);

void *memory_set(void *dest, int32_t value, uint64_t size);

char *memory_get_usage();
