#pragma once
#include "simpleTypes.h"

typedef enum TOKEN_ENUM {
    TOKEN_KEYW_LOAD,
    TOKEN_KEYW_TICK,
    TOKEN_KEYW_FUNCTION,
    TOKEN_KEYW_WHILE,
    TOKEN_KEYW_IF,
    TOKEN_KEYW_ELSE,
    TOKEN_KEYW_NAMESPACE,
    /* TOKEN_IDENTIFIER, */
    /* TOKEN_INT, */
    /* TOKEN_PLUS, */
    /* TOKEN_MINUS, */
    /* TOKEN_MULTIPLY, */
    /* TOKEN_DIVIDE, */
    TOKEN_STRING,
    TOKEN_RAW_AREA,
    TOKEN_MULTILINE_CMD,
    TOKEN_SEMICOLON,
    TOKEN_OPEN_P, // parentheses
    TOKEN_CLOSE_P,
    TOKEN_OPEN_CB, // curly brace
    TOKEN_CLOSE_CB,
    
} token_E;

typedef struct TOKEN_STRUCT {
  token_E type;
  union {
    string_T str;
  } value;
} token_T;
