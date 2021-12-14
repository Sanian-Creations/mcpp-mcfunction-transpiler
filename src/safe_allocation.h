#pragma once
#include <stddef.h>


void free(void* ptr);
// This declaration is only here so we don't have to include
// stdlib.h which contains the unsafe allocation functions,
// and we're precisely trying to avoid using those

void* safe_calloc(size_t n);
void* safe_malloc(size_t n);
void* safe_realloc(void* ptr, size_t n);
