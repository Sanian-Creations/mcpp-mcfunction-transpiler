#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "simpleTypes.h"
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

#ifdef SYNTAX_FILE // I don't care if you're not supposed to use header files like this
static void define_keyword(char* word, token_E token_type);
static void define_comment(char* start, char* end, bool eof_terminated);

static size_t keyword_count = 0;
static size_t comment_count = 0;
comment_definition_T comments[2]; // ! remember to export
keyword_definition_T keywords[8];
const size_t comments_len = sizeof(comments) / sizeof(comment_definition_T);
const size_t keywords_len = sizeof(keywords) / sizeof(keyword_definition_T);

bool is_syntax_defined = false;


#else // EXPORTS
extern bool is_syntax_defined;
extern comment_definition_T comments[];
extern keyword_definition_T keywords[];
extern size_t comments_len;
extern size_t keywords_len;
#endif
