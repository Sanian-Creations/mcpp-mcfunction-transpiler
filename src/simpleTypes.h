#pragma once
#include <string.h>

// does not have to be 0 terminated, can represent a subsection of a larger string.
typedef struct STRING_STRUCT {
  char* chars;
  size_t len;
} string_T;

void string_init(string_T* string, char* chars);
void print_string(string_T* string);
