#pragma once

#include <stdint.h>

struct vector {
  void *data;
  uint64_t size;
  uint64_t capacity;
  uint64_t element_size;
};

void vector_init(struct vector *vector, uint64_t element_size);
int vector_resize(struct vector *vector, uint64_t size);
int vector_pushback(struct vector *v, void *element);
void *vector_at(struct vector *vector, uint64_t index);
void vector_free(struct vector *vector);
