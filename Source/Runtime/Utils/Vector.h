#pragma once

#include <stdint.h>

struct Vector {
  void *data;
  uint64_t size;
  uint64_t capacity;
  uint64_t element_size;
};

void vector_init(struct Vector *vector, uint64_t element_size);
int vector_resize(struct Vector *vector, uint64_t size);
int vector_pushback(struct Vector *v, void *element);
void *vector_at(struct Vector *vector, uint64_t index);
void vector_free(struct Vector *vector);
