#define LEXER_FILE
#include "lexer.h"

void lex(string_T src, dataList_T* tokens, char** error) {

  *error = 0;
  if (!is_syntax_defined) {
    define_syntax();
  }

  lexer_T lexer;
  lexer_init(&lexer, src);
  dataList_init(tokens, 2048, sizeof(token_T));
  
  lex_continue:
  while (lexer.i < lexer.src.len) {

    // 1. move past all whitespace
    skip_whitespace(&lexer);
    if (lexer.i >= lexer.src.len) break;

    
    // 2. move past comments
    for (size_t i = 0; i < comments_len; i++) {      
      if (match_here(&lexer, &comments[i].start)) {
	printf("\"%s\" on line %u, ", comments[i].start.chars, lexer.line);
	forward_by(&lexer, comments[i].start.len);
	skip_until(&lexer, &comments[i].end);
	printf("skipped to line %u\n", lexer.line);
	goto lex_continue;
      }
    }
    
    // 3. (key)words
    if (is_lowercase_letter(lexer.c)) {
      string_T word;
      word.len = 0;
      word.chars = &lexer.src.chars[lexer.i];
      do {
	forward(&lexer);
	word.len++;
      } while (is_lowercase_letter(lexer.c));

      // Turn these into either keyword tokens or identifier tokens
      // for now just print them real nice so we know it works :)
      putchar('"');
      print_string(&word);
      printf("\" ");
      
      goto lex_continue;
    }
    
    // 4. strings/contentblocks
    // 5. static symbols (operators, braces, etc.)
    
    printf("\nUnknown character: '%c', character code %d\n", lexer.c, (int) lexer.c);
    *error = "Unknown character.";
    return;
  }

  // reached end of file
  return;
}


static void forward(lexer_T* lexer) {
  // no bounds checking.
  if (lexer->c == '\n') lexer->line++;
  lexer->i++;
  lexer->c = lexer->src.chars[lexer->i];
}

static void forward_by(lexer_T* lexer, size_t offset) {
  // assumes the offset is within bounds
  for (offset += lexer->i; lexer->i < offset; lexer->i++) {
    if (lexer->src.chars[lexer->i] == '\n') lexer->line++;
  }
  lexer->c = lexer->src.chars[lexer->i];
}

static char peek(lexer_T* lexer, size_t offset) {
  size_t index = lexer->i + offset;
  if (index >= lexer->src.len) return 0; // must be in-bounds  
  return lexer->src.chars[index];
}

static bool match_here(lexer_T* lexer, string_T* string) {
  if (lexer->c != string->chars[0]) return false;
  for (size_t i = 1; i < string->len; i++) {
    if (peek(lexer, i) != string->chars[i]) {
      return false;
    }
  }
  return true;
}

static void skip_until(lexer_T* lexer, string_T* str) {
  while (lexer->i < lexer->src.len) {
    if (match_here(lexer, str)) {
      forward_by(lexer, str->len); // move past it, too
      return;
    }
    forward(lexer);
  }
}

static void skip_whitespace(lexer_T* lexer) {
  while(1) {
    switch(lexer->c){
    case '\n':
    case ' ':
    case '\t':
    case '\r':
    case '\v':
      forward(lexer);
      continue;
    default:
      return;
    }
  }
}

static bool is_lowercase_letter(char c) {
  return c >= 'a' && c <= 'z';
}

static bool is_identifier_allowed(char c) {
  return // [a-z0-9/._-]
    (c >= 'a' && c <= 'z') ||
    (c >= '0' && c <= '9') ||
    c == '.' || c == '_' || c == '-';
}

void lexer_init(lexer_T* lexer, string_T src) {
  lexer->src = src;
  lexer->i = 0;
  lexer->line = 1;
  lexer->c = src.chars[0];
}
