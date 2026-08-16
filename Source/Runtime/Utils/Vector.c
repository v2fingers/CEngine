#include "Vector.h"
#include "Memory.h"

void vector_init(struct Vector *vector, uint64_t element_size) {
  vector->data = nullptr;
  vector->size = 0;
  vector->capacity = 0;
  vector->element_size = element_size;
}

int vector_resize(struct Vector *vector, uint64_t size) {
  if (size > vector->capacity) {
    void *new_data =
        memory_reallocate(vector->data, vector->capacity * vector->element_size,
                          size * vector->element_size);

    if (!new_data) {
      return 0;
    }

    vector->data = new_data;
    vector->capacity = size;
  }

  if (size > vector->size) {
    memory_set((char *)vector->data + vector->size * vector->element_size, 0,
               (size - vector->size) * vector->element_size);
  }

  vector->size = size;
  return 1;
}

int vector_pushback(struct Vector *v, void *element) {
  if (v->size >= v->capacity) {
    uint64_t old_capacity = v->capacity;
    uint64_t new_capacity = v->capacity == 0 ? 4 : v->capacity * 2;

    void *new_data = memory_reallocate(v->data, old_capacity * v->element_size,
                                       new_capacity * v->element_size);

    if (!new_data) {
      return 0;
    }

    v->data = new_data;
    v->capacity = new_capacity;
  }

  memory_copy((char *)v->data + v->size * v->element_size, element,
              v->element_size);

  v->size++;
  return 1;
}

void *vector_at(struct Vector *vector, uint64_t index) {
  return (char *)vector->data + index * vector->element_size;
}

void vector_free(struct Vector *vector) {
  memory_free(vector->data, sizeof(vector->data));

  vector->data = nullptr;
  vector->size = 0;
  vector->capacity = 0;
  vector->element_size = 0;
}
