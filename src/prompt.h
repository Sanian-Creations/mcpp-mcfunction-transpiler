#pragma once
#include <stddef.h>
#include <stdbool.h>

bool prompt_yes_no(const char* message);
size_t get_limited_input(char* buf, size_t limit, bool null_terminated);
