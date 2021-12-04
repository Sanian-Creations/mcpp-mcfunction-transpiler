#include "simpleTypes.h"

void string_init(string_T* string, char* chars) {
  string->chars = chars;
  string->len = strlen(chars);
}

void print_string(string_T* string) {
  fwrite(string->chars, 1, string->len, stdout);
}

bool string_match(string_T* a, string_T* b) {
  if (a->len != b->len) return false;

  // Gonna compare the strings 8 bytes at a time for EFFICIENCY
  // I think. I vaguely remember that doing something like this
  // is better than doing it byte by byte. Maybe.
  size_t bigsteps = a->len / sizeof(long long);
  size_t smallsteps = a->len % sizeof(long long);
  
  for (size_t i = 0; i < bigsteps; i++) {
    if (((long long*)a->chars)[i] != ((long long*)b->chars)[i])
      return false;
  }
  
  for (size_t i = a->len - smallsteps; i < a->len; i++) {
    if (a->chars[i] != b->chars[i])
      return false;
  }
  
  return true;
}
