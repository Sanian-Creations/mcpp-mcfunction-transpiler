#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "simpleTypes.h" 

string_T fileToStr(const char* fileName, char** errorMsg);
size_t fileSize(FILE* file);
