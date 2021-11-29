#pragma once
#include <stdlib.h>

// -----

typedef struct POINTERLIST_TYPE {
  void** arr; // array of pointers
  size_t len;
  size_t capacity;
} pointerList_T;

void pointerList_init(pointerList_T* list, size_t capacity);
void pointerList_add(pointerList_T* list, void* elem);
void pointerList_dispose(pointerList_T* list);
void* pointerList_pop(pointerList_T* list);

// -----

typedef struct DATALIST_TYPE {
  char* arr; // byte array
  size_t elemSize;
  size_t len;
  size_t capacity;
} dataList_T;

void dataList_init(dataList_T* list, size_t capacity, size_t elemSize);
void* dataList_add(dataList_T* list);
void dataList_dispose(dataList_T* list);

// get the pointer to the correct place, but cast it to void so
// that you can't just forget to cast when using this macro
#define DATALIST_VAL_GET(l, i) ((void*)(l.arr + (i * l.elemSize)))
#define DATALIST_REF_GET(l, i) ((void*)(l->arr + (i * l->elemSize)))

// ----
