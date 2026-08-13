#pragma once

#include <stdint.h>

typedef struct Vector {
  void *data;
  uint64_t size;
  uint64_t capacity;
  uint64_t element_size;
} Vector_t;

void VectorInit(Vector_t *vector, uint64_t element_size);
int VectorResize(Vector_t *v, uint64_t size);
int VectorPushBack(Vector_t *v, void *element);
void *VectorAt(Vector_t *vector, uint64_t index);
void VectorFree(Vector_t *vector);
