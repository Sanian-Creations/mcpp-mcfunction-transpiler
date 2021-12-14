#pragma once
#include <stdbool.h>
#include "token.h"

void define_syntax();

typedef struct COMMENT_DEFINITION_STRUCT {
  string_T start;
  string_T end;
  bool eof_terminated; // whether reaching the eof is a valid way to terminate this comment.
} comment_definition_T;

typedef struct KEYWORD_DEFINITION_STRUCT {
  string_T word;
  token_E token_type;
} keyword_definition_T;

#ifndef SYNTAX_FILE 
extern bool is_syntax_defined;
extern comment_definition_T comments[];
extern keyword_definition_T keywords[];
extern size_t comments_len;
extern size_t keywords_len;
#endif
