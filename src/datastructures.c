#include "datastructures.h"
#include "safe_allocation.h"

/*
  POINTER LIST
*/

void pointerList_init(pointerList_T* list, size_t capacity) {
  list->capacity = capacity;
  list->len = 0;

  if (capacity == 0) {
    list->arr = 0; // clean mem pointer
    return;
  }
  
  list->arr = safe_malloc(capacity * sizeof(void*));
}

void pointerList_add(pointerList_T* list, void* elem) {
  if (list->len >= list->capacity) { // resize required
    list->capacity *= 2;
    list->arr = safe_realloc(list->arr, list->capacity * sizeof(void*));
  }
  
  list->arr[list->len++] = elem;
}

void pointerList_dispose(pointerList_T* list) {
  free(list->arr);
  list->arr = 0; // clean mem pointer.
}

void* pointerList_pop(pointerList_T* list) {
  if (list->len <= 0) return 0;

  return list->arr[--list->len];
}

/*
  DATA_LIST
*/

void dataList_init(dataList_T* list, size_t capacity, size_t elemSize){
  list->capacity = capacity;
  list->elemSize = elemSize;
  list->len = 0;

  if (capacity == 0 || elemSize == 0) {
    list->arr = 0; // clean mem pointer
    return;
  }
  
  list->arr = safe_malloc(capacity * elemSize);
}


// doesnt actually add anything, simply increases list.len and returns a pointer to the new spot so that you may add stuff yourself
void* dataList_add(dataList_T* list) {
  // both len and capacity count elements, they do not need to be multiplied
  // by elemSize before they are compared
  
  if (list->len >= list->capacity) { // resize required
    list->capacity *= 2;
    list->arr = safe_realloc(list->arr, list->capacity * list->elemSize);
  }
  
  return DATALIST_REF_GET(list, list->len++);
}

void dataList_dispose(dataList_T* list) {
  free(list->arr);
  list->arr = 0; // clean mem pointer
}
