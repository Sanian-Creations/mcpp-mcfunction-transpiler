#include <stdlib.h>
#include <stdio.h>
#include "safe_allocation.h"
#include "macros.h"

#define EXIT_IF_ALLOCATED_NULL(p, n, f) if (p == NULL) { \
    printf("Out of memory. Tried to allocate %"SIZE_T_FORMAT" bytes, %s returned NULL.", n, f); \
    exit(1); \
}

void* safe_calloc(size_t n) {
  void* new_ptr = calloc(n, 1);
  EXIT_IF_ALLOCATED_NULL(new_ptr, n, "calloc");
  return new_ptr;
}

void* safe_malloc(size_t n) {
  void* new_ptr = malloc(n);
  EXIT_IF_ALLOCATED_NULL(new_ptr, n, "malloc");
  return new_ptr;
}

void* safe_realloc(void* ptr, size_t n) {
  void* new_ptr = realloc(ptr, n);
  EXIT_IF_ALLOCATED_NULL(new_ptr, n, "realloc");
  return new_ptr;
}
