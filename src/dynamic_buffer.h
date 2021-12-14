#pragma once
#include <stddef.h>

typedef struct DYNAMIC_BUFFER_STRUCT {
  size_t capacity;
  size_t len;
  void* arr;
} dynamic_buffer_T;

void dynamic_buffer_init(dynamic_buffer_T* buffer, size_t capacity);
void dynamic_buffer_append(dynamic_buffer_T* buffer, void* ptr, size_t len);
void dynamic_buffer_append_char(dynamic_buffer_T* buffer, char c);
void dynamic_buffer_dispose(dynamic_buffer_T* buffer);
void* dynamic_buffer_export(dynamic_buffer_T* buffer);
