#pragma once
#include "simpleTypes.h" 
#include <stdio.h>

string_T file_to_string(const char* fileName, char** errorMsg);
size_t file_size(FILE* file);
