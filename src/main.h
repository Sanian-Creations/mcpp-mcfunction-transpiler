#pragma once
#ifndef MCPP_ENTRYPOINT
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "lexer.h"
#include "readFile.h" 

enum InputFlag {
  // flags are powers of 2 so that they are each a single bit
  UNKNOWN_FLAG   = 0,
  FUNCTIONS_ONLY = 1,
  A = 2,
  B = 4,
  C = 8,
  D = 16,
  E = 32,
  F = 64,
  G = 128,
  H = 256,
  I = 512,
  J = 1024,
  K = 2048,
};

typedef struct ARG_OPTIONS_STRUCT{
  char* file;
  int flags;
} argOptions_T;

void parseArgs(int argc, char** args, argOptions_T* options, char** error);
bool isFlag(const char* str);
enum InputFlag getFlag(const char* str);
#endif

int real_main(int argCount, char** args);
