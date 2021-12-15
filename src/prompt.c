#include "prompt.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static char to_lower(char c) {
  return (c >= 'A' && c <= 'Z') ? c - ('A' - 'a') : c;
}

size_t get_limited_input(char* buf, size_t limit, bool null_terminated) {

  if (null_terminated) {
    limit--;
    buf[limit] = 0;
  }

  int c;
  for (size_t i = 0; i < limit; i++) {
    c = fgetc(stdin);
    if (c == '\n' || c == EOF) {
      if (null_terminated) buf[i] = 0;
      return i;
    }
    buf[i] = (char)c;
  }

  do { // consume any leftover characters
    c = fgetc(stdin);
  } while (!(c == '\n' || c == EOF));

  return limit;
}

bool prompt_yes_no(const char* message) {
  const int bufsize = 3;
  char buf[bufsize];

  while (1) {
    printf("%s [y/n]: ", message);
    size_t input_len = get_limited_input(buf, bufsize, false);

    for (size_t i = 0; i < input_len; i++) buf[i] = to_lower(buf[i]);

    if (input_len == 1) {
      if (buf[0] == 'y') return true;
      if (buf[0] == 'n') return false;
    } else {
      if (input_len == 3 && memcmp(buf, "yes", 3) == 0) return true;
      if (input_len == 2 && memcmp(buf, "no",  2) == 0) return false;
    }

    printf("Invalid input. Type 'y' or 'n'.\n");
  }
}
