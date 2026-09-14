#pragma once
#include "Defines.h"

typedef struct dynamic_array {
  void *data;
  i32 size;
  i32 capacity;
  u64 element_size;
} DArray;

void darray_init(DArray *array, i32 initial_capacity, u64 element_size);
void darray_insert(DArray *array, void *element);
void *darray_get(DArray *array, i32 index);
void darray_free(DArray *darray);
