#include "Vector.h"
#include "Memory.h"

void V_VectorInit(Vector_t *vector, uint64_t element_size) {
  vector->data = nullptr;
  vector->size = 0;
  vector->capacity = 0;
  vector->element_size = element_size;
}

int VectorResize(Vector_t *v, uint64_t size) {
  if (size > v->capacity) {
    void *new_data = ReAllocate(v->data, v->capacity * v->element_size,
                                size * v->element_size);

    if (!new_data) {
      return 0;
    }

    v->data = new_data;
    v->capacity = size;
  }

  if (size > v->size) {
    SetMemory((char *)v->data + v->size * v->element_size, 0,
              (size - v->size) * v->element_size);
  }

  v->size = size;
  return 1;
}

int VectorPushBack(Vector_t *v, void *element) {
  if (v->size >= v->capacity) {
    uint64_t old_capacity = v->capacity;
    uint64_t new_capacity = v->capacity == 0 ? 4 : v->capacity * 2;

    void *new_data = ReAllocate(v->data, old_capacity * v->element_size,
                                new_capacity * v->element_size);

    if (!new_data) {
      return 0;
    }

    v->data = new_data;
    v->capacity = new_capacity;
  }

  CopyMemory((char *)v->data + v->size * v->element_size, element,
             v->element_size);

  v->size++;
  return 1;
}

void *VectorAt(Vector_t *vector, uint64_t index) {
  return (char *)vector->data + index * vector->element_size;
}

void VectorFree(Vector_t *vector) {
  Free(vector->data, sizeof(vector->data));

  vector->data = nullptr;
  vector->size = 0;
  vector->capacity = 0;
  vector->element_size = 0;
}
