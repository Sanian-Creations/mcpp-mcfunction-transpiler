#pragma once

#include <stdbool.h>

int make_directory(const char* name);
bool file_exists(const char* name);
bool is_regular_file(const char *path);
bool is_directory(const char *path);
int remove_rf(char *path);
