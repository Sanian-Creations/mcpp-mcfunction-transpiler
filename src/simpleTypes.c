#include "simpleTypes.h"
#include <stdio.h>

void string_init(string_T* string, char* chars) {
  string->chars = chars;
  string->len = strlen(chars);
}

void print_string(string_T* string) {
  fwrite(string->chars, 1, string->len, stdout);
}
