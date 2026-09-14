#include "DArray.h"
#include "Core/Logger.h"
#include "Core/Memory/Memory.h"
#include <string.h>

// Initialize the array
void darray_init(DArray *darray, i32 initialCapacity, u64 elementSize) {

  darray->data = mem_alloc(initialCapacity * elementSize);
  darray->size = 0;
  darray->capacity = initialCapacity;
  darray->element_size = elementSize;
}

// Add an element, growing the array if necessary
void darray_insert(DArray *darray, void *element) {
  if (darray->size == darray->capacity) {
    i32 old_cap = darray->capacity;
    darray->capacity *= 2;

    void *temp = mem_realloc(darray->data, old_cap, darray->capacity * darray->element_size);

    if (temp == NULL) {
      LOG_ERROR("Memory reallocation failed!");
      return;
    }

    darray->data = temp;
  }

  // Find the address of the next element
  void *destination = (char *)darray->data + darray->size * darray->element_size;

  // Copy the element into the array
  mem_copy(destination, element, darray->element_size);

  darray->size++;
}

// Get an element by index
void *darray_get(DArray *darray, i32 index) {
  return (char *)darray->data + index * darray->element_size;
}

// Clean up memory
void darray_free(DArray *darray) {
  mem_free(darray->data, sizeof(darray->data));

  darray->data = NULL;
  darray->size = 0;
  darray->capacity = 0;
  darray->element_size = 0;
}
