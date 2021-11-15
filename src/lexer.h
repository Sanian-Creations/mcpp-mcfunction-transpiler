#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// does not have to be 0 terminated, can represent a subsection of a larger string.
typedef struct STRING_STRUCT {
  char* str;
  size_t len;
} string_T;

typedef struct LEXER_STRUCT {
  string_T src;
  size_t i;
  char c;
} lexer_T;

typedef struct TOKEN_STRUCT {
  enum {
    TOKEN_KEYW_LOAD,
    TOKEN_KEYW_TICK,
    TOKEN_KEYW_FUNCTION,
    TOKEN_KEYW_WHILE,
    TOKEN_KEYW_IF,
    TOKEN_KEYW_ELSE,
    TOKEN_KEYW_NAMESPACE,
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_OPEN_P, // parentheses
    TOKEN_CLOSE_P,
    TOKEN_OPEN_CB, // curly brace
    TOKEN_CLOSE_CB,
    TOKEN_STRING,
    TOKEN_MULTILINE_CMD,
    TOKEN_RAW_AREA
  } type;
  union {
    string_T str;
  } value;
} token_T;


string_T fileToStr(const char* fileName, char** errorMsg);
size_t fileSize(FILE* file);
bool strEq(const char* str1, const char* str2);

token_T* lex(string_T src, char** error);
void init(lexer_T* lexer, string_T src);
