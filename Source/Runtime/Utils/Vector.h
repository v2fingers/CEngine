#pragma once

#include <stdint.h>

typedef struct {
  void *data;
  uint64_t size;
  uint64_t capacity;
  uint64_t element_size;
} Vector;

void vector_init(Vector *vector, uint64_t element_size);
int vector_resize(Vector *vector, uint64_t size);
int vector_pushback(Vector *v, void *element);
void *vector_at(Vector *vector, uint64_t index);
void vector_free(Vector *vector);
