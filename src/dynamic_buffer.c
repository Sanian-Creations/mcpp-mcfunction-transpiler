#include "dynamic_buffer.h"
#include "safe_allocation.h"
#include <string.h>

void dynamic_buffer_init(dynamic_buffer_T* buffer, size_t capacity) {
  buffer->capacity = capacity;
  buffer->len = 0;
  buffer->arr = safe_malloc(capacity);
}

void dynamic_buffer_append(dynamic_buffer_T* buffer, void* ptr, size_t n) {
  if (buffer->len + n > buffer->capacity) {
    do {
      buffer->capacity *= 2;
      // one resize might not be enough, repeat if necessary
    } while (buffer->len + n > buffer->capacity);
    
    buffer->arr = safe_realloc(buffer->arr, buffer->capacity);
  }

  memcpy(&((char*)buffer->arr)[buffer->len], ptr, n);
  buffer->len += n;
}

void dynamic_buffer_append_char(dynamic_buffer_T* buffer, char c) {
  if (buffer->len >= buffer->capacity) {
    buffer->capacity *= 2;
    buffer->arr = safe_realloc(buffer->arr, buffer->capacity);
  }

  ((char*)buffer->arr)[buffer->len++] = c;
}

void dynamic_buffer_dispose(dynamic_buffer_T* buffer) {
  free(buffer->arr);
  buffer->arr = 0;
  buffer->len = 0;
  buffer->capacity = 0;
}

void* dynamic_buffer_export(dynamic_buffer_T* buffer) {
  void* export = safe_malloc(buffer->len);
  
  memcpy(export, buffer->arr, buffer->len);
  return export;
}
