#include "DArray.h"
#include "Core/Logger.h"
#include <string.h>

// Initialize the array
void darray_init(DArray *array, i32 initialCapacity, u64 elementSize) {

  array->data = malloc(initialCapacity * elementSize);
  array->size = 0;
  array->capacity = initialCapacity;
  array->element_size = elementSize;
}

// Add an element, growing the array if necessary
void darray_insert(DArray *array, void *element) {
  if (array->size == array->capacity) {
    array->capacity *= 2;

    void *temp = realloc(array->data, array->capacity * array->element_size);

    if (temp == NULL) {
      LOG_ERROR("Memory reallocation failed!");
      return;
    }

    array->data = temp;
  }

  // Find the address of the next element
  void *destination = (char *)array->data + array->size * array->element_size;

  // Copy the element into the array
  memcpy(destination, element, array->element_size);

  array->size++;
}

// Get an element by index
void *darray_get(DArray *array, i32 index) {
  return (char *)array->data + index * array->element_size;
}

// Clean up memory
void darray_free(DArray *array) {
  free(array->data);

  array->data = NULL;
  array->size = 0;
  array->capacity = 0;
  array->element_size = 0;
}
