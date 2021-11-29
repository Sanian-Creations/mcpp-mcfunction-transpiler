#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "simpleTypes.h"
#include "datastructures.h"
#include "token.h"
#include "syntax.h"


typedef struct LEXER_STRUCT {
  string_T src;
  size_t i;
  size_t line;
  char c;
} lexer_T;
void lexer_init(lexer_T* lexer, string_T src);

void define_syntax();
void lex(string_T src, dataList_T* tokens, char** error);


#ifdef LEXER_FILE
static void forward(lexer_T* lexer);
static void forward_by(lexer_T* lexer, size_t offset);
static char peek(lexer_T* lexer, size_t offset);
static bool match_here(lexer_T* lexer, string_T* matcher);
static void skip_until(lexer_T* lexer, string_T* str);
static void skip_whitespace(lexer_T* lexer);
static bool is_lowercase_letter(char c);
#endif
