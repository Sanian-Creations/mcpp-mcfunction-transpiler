#include "simpleTypes.h"
#include <stdio.h>
#include <string.h>

void string_init(string_T* string, char* chars) {
  string->chars = chars;
  string->len = strlen(chars);
}

void print_string(string_T* string) {
  fwrite(string->chars, 1, string->len, stdout);
}

bool string_match(string_T* a, string_T* b) {
  if (a->len != b->len) return false;

  return memcmp(a->chars, b->chars, a->len) == 0;
}
