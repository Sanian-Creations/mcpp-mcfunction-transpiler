#pragma once
#include <stdbool.h>
#include "simpleTypes.h"
#include "datastructures.h"



typedef struct LEXER_STRUCT {
  string_T src;
  size_t i;
  size_t line;
  char c;
} lexer_T;
void lexer_init(lexer_T* lexer, string_T src);

void define_syntax();
void lex_src(string_T src, dataList_T* tokens, char** error);
