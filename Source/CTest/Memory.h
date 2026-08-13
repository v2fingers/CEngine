#pragma once
#include <stdint.h>

void InitializeMemory();
void ShutdownMemory();

void *ZeroMemory(void *chunk, uint64_t size);
void *CopyMemory(void *dest, const void *source, uint64_t size);

void *Allocate(uint64_t size);
void *ReAllocate(void *chunk, uint64_t size, uint64_t newsize);
void Free(void *chunk, uint64_t size);

void *SetMemory(void *dest, int32_t value, uint64_t size);

char *GetMemoryUsage();
