#define LEXER_FILE
#include <stdio.h>
#include "lexer.h"
#include "syntax.h"
#include "macros.h"
#include "dynamic_buffer.h"

static void forward(lexer_T* lexer);
static void forward_by(lexer_T* lexer, size_t offset);
static char peek(lexer_T* lexer, size_t offset);
static char peek_unsafe(lexer_T* lexer, size_t offset);
static bool match_here(lexer_T* lexer, string_T* matcher);
static bool skip_until_past(lexer_T* lexer, string_T* str);
static bool skip_until_past_char(lexer_T* lexer, char c);
static void skip_whitespace(lexer_T* lexer);
static bool is_lowercase_letter(char c);
static bool skip_comments(lexer_T* lexer, char** error);
static bool find_keyword(lexer_T* lexer, dataList_T* tokens);
static bool find_string(lexer_T* lexer, dataList_T* tokens, char** error);
static bool find_raw_area(lexer_T* lexer, dataList_T* tokens, char** error);
static bool find_command(lexer_T* lexer, dataList_T* tokens, char** error);
static bool find_symbol(lexer_T* lexer, dataList_T* tokens);
static char* current_char_ptr(lexer_T* lexer);

static dynamic_buffer_T str_buffer = {0};

void lex_src(string_T src, dataList_T* tokens, char** error) {

  if (!is_syntax_defined) {
    define_syntax();
  }

  lexer_T lexer;
  lexer_init(&lexer, src);
  dataList_init(tokens, 2048, sizeof(token_T));

  *error = 0;

  while (lexer.c) {

    skip_whitespace(&lexer);
    if (lexer.c == 0) return;

    // Whenever one of these functions find something, they must also
    // move past it. If they do not, then this loop keeps going eternally
    bool found_anything =
      skip_comments(&lexer, error) ||
      find_keyword(&lexer, tokens) ||
      find_string(&lexer, tokens, error) ||
      find_command(&lexer, tokens, error) ||
      find_raw_area(&lexer, tokens, error) ||
      find_symbol(&lexer, tokens);

    if (*error) return;

    if (!found_anything) {
      printf("Unknown character '%c' (ASCII %d) on line %"SIZE_T_FORMAT"\n", lexer.c, (int)lexer.c, lexer.line);
      *error = "Unknown character.";
      return;
    }
  } // end of while-loop
} // end of lex()

// Makes the lexer go forward by one
// Increases the line count after passing a newline
// Sets lexer.c to 0 after reaching eof,
// Doesn't allow reading outside of the bounds of the string.
static void forward(lexer_T* lexer) {
  if (lexer->c == '\n') lexer->line++;

  lexer->c = (++(lexer->i) < lexer->src.len)
    ? lexer->src.chars[lexer->i]
    : 0;
}

// Makes the lexer go forward by offset
// Increases the line count after passing a newline
// Sets lexer.c to 0 after reaching eof
static void forward_by(lexer_T* lexer, size_t offset) {
  offset += lexer->i;

  // Bounds check
  if (offset >= lexer->src.len) {
    lexer->i = lexer->src.len;
    lexer->c = 0;
    return;
  }

  // Move whilst counting newlines
  for (;lexer->i < offset; lexer->i++) {
    if (lexer->src.chars[lexer->i] == '\n') lexer->line++;
  }

  // Set lexer.c
  lexer->c = lexer->src.chars[lexer->i];
}

// Looks ahead (or back) by 'offset' and returns the character at that index.
// Returns 0 if the index is out of bounds.
static char peek(lexer_T* lexer, size_t offset) {
  offset += lexer->i;
  if (offset >= lexer->src.len) return 0;
  return lexer->src.chars[offset];
}

// Looks ahead (or back) by 'offset' and returns the character at that index.
// Does no bounds checking!
static char peek_unsafe(lexer_T* lexer, size_t offset) {
  return lexer->src.chars[lexer->i + offset];
}

// Checks if the given string is at the current location of the lexer
static bool match_here(lexer_T* lexer, string_T* string) {
  // If string would go out of bounds, it can't be a match
  if (lexer->i + string->len >= lexer->src.len) {
    return false;
  }

  // Compare against all characters of the string
  for (size_t i = 0; i < string->len; i++) {
    if (peek_unsafe(lexer, i) != string->chars[i]) {
      return false;
    }
  }

  return true;
}

// From the current location, searches for the next occurence of the
// given string, then places the lexer pointer right after it.
// Returns false when the string was never found before reaching oef.
static bool skip_until_past(lexer_T* lexer, string_T* str) {
  while (lexer->c) {
    if (lexer->c == str->chars[0] && match_here(lexer, str)) { // first check is to not go into the function unnecessarily
      forward_by(lexer, str->len); // move past it, too
      return true;
    }
    forward(lexer);
  }
  return false;
}

// From the current location, searches for the next occurence of the
// given character, then places the lexer pointer right after it.
// Returns false when the string was never found before reaching oef.
static bool skip_until_past_char(lexer_T* lexer, char c) {
  while (lexer->c) {
    if (lexer->c == c) {
      forward(lexer); // move past it, too
      return true;
    }
    forward(lexer);
  }
  return false;
}

// Places the pointer on the first non-whitespace character it sees.
// Doesn't move if the current character is already non-whitespace.
static void skip_whitespace(lexer_T* lexer) {
  // whitespace: tab, linefeed, vert tab, form feed, carriage return, space
  // ascii:      9    10        11        12         13               32
  while((lexer->c >= '\t' && lexer->c <= '\r') || lexer->c == ' ') {
    forward(lexer);
  }
}

static bool is_lowercase_letter(char c) {
  return c >= 'a' && c <= 'z';
}

// When at the start of a comment, moves completely past it.
// Returns false when the lexer isn't at the start of a comment.
static bool skip_comments(lexer_T* lexer, char** error) {
  size_t start = lexer->line;

  for (size_t i = 0; i < comments_len; i++) {

    if (!match_here(lexer, &comments[i].start)) continue;

    forward_by(lexer, comments[i].start.len); // move past start of comment

    bool found_comment_end = skip_until_past(lexer, &comments[i].end);
    if (found_comment_end || comments[i].eof_terminated) {
      return true; // found a valid comment
    }

    // skip_until_past reached eof, but eof isn't a valid way to terminate this type of comment.
    printf("Unterminated comment starting on line %"SIZE_T_FORMAT"\n", start);

    *error = "Unterminated comment.";
    return true; // found invalid comment and reached eof.
  }

  return false; // no comment found.
}

static bool find_keyword(lexer_T* lexer, dataList_T* tokens) {
  if (!is_lowercase_letter(lexer->c)) return false; // found no word

  // mark the start of the word
  string_T word = {0};
  word.chars = current_char_ptr(lexer);

  // loop until end of word
  do {
    forward(lexer);
    word.len++;
  } while (is_lowercase_letter(lexer->c)); // will also stop when reaching eof

  // compare the word against known keywords
  for (size_t i = 0; i < keywords_len; i++) {
    if (string_match(&(keywords[i].word), &word)) {
      token_T* token = dataList_add(tokens);
      token->type = keywords[i].token_type;

      printf("keyword: "); // debug prints
      print_string(&word);
      putchar('\n');

      return true; // found keyword
    }
  }

  return true; // found a word, but not a keyword. We ignore this for now.
}

static bool find_string(lexer_T* lexer, dataList_T* tokens, char** error) {
  if (lexer->c != '"') return false;

  forward(lexer); // move to the first character of the string

  // This string won't be 'processed' yet (no unescaping the characters)
  // simply mark the starting position and its length in the source string
  token_T* token = dataList_add(tokens);
  token->type = TOKEN_STRING;
  token->value.str.chars = current_char_ptr(lexer);
  token->value.str.len = 0;

  while (lexer->c && lexer->c != '\n') { // newlines and eof = bad
    if (lexer->c == '"' && peek(lexer, -1) != '\\') {
      forward(lexer); // move past the ending quote

      printf("string: \""); // print it real nice for testing
      print_string(&token->value.str);
      printf("\"\n");
      return true;
    }
    forward(lexer);
    token->value.str.len++;
  }

  printf("\nInvalid string on line %"SIZE_T_FORMAT"\n", lexer->line);
  *error = "Invalid string, missing endquote. Strings may not span multiple lines.";
  return true;
}

static bool find_raw_area(lexer_T* lexer, dataList_T* tokens, char** error) {

  //  start: @{
  if (!(lexer->c == '@' && peek(lexer, 1) == '{')) return false;

  forward_by(lexer, 2); // move into the area
  size_t start_line = lexer->line;
  skip_whitespace(lexer); // goes to the first character in the area

  if (lexer->c == '}' && peek(lexer, 1) == '@') {
    // found an area, but it was empty. Don't need to add a token for it.
    forward_by(lexer, 2); // move out of the area
    return true;
  }

  // Start actually processing the raw area
  token_T* token = dataList_add(tokens);
  token->type = TOKEN_RAW_AREA;
  token->value.str.chars = 0;

  if (!str_buffer.arr) dynamic_buffer_init(&str_buffer, 1024);
  str_buffer.len = 0; // reset the string processor.
  
  //  end: newline, optional whitespace, }@
  while (lexer->c) {
    char* first_char_of_line = current_char_ptr(lexer);
    size_t first_char_index = lexer->i;
    skip_until_past_char(lexer, '\n');
    
    dynamic_buffer_append(&str_buffer, first_char_of_line, (lexer->i - first_char_index) * sizeof(char));
    
    skip_whitespace(lexer);

    if (lexer->c == '}' && peek(lexer, 1) == '@') {
      token->value.str.len = str_buffer.len / sizeof(char);
      token->value.str.chars = dynamic_buffer_export(&str_buffer);
      forward_by(lexer, 2); // move out of the area

      
      printf("Raw block: line %"SIZE_T_FORMAT" to %"SIZE_T_FORMAT"\n---\n", start_line, lexer->line);
      print_string(&token->value.str);
      printf("---\n");
      
      return true;
    }
  }

  printf("Unterminated raw area on line %"SIZE_T_FORMAT".", start_line);
  *error = "Unterminated raw area.";
  return true;
}

static bool find_command(lexer_T* lexer, dataList_T* tokens, char** error) {
  if (lexer->c != '/') return false;

  forward(lexer); // move into the command

  token_T* token = dataList_add(tokens);
  token->type = TOKEN_MULTILINE_CMD;
  token->value.str.chars = current_char_ptr(lexer);
  size_t start_line = lexer->line;

  while (lexer->c) {
    if (lexer->c == ';' && peek(lexer, -1) != '\\') { // only an unescaped ; ends the command. 
      token->value.str.len = lexer->i - start_line;
      forward(lexer); // move out of the command

      printf("Command: line %"SIZE_T_FORMAT" to %"SIZE_T_FORMAT"\n", start_line, lexer->line);

      return true;
    }

    if (skip_comments(lexer, error)) continue;

    if (lexer->c == '"') { // String within the command.
      do { // initial iteration steps into the string
	forward(lexer);
	// Exit upon reaching eof or unescaped double quote.
      } while (lexer->c && !(lexer->c == '"' && peek(lexer, -1) != '\\'));
    }

    forward(lexer);
  }

  printf("Unterminated command on line %"SIZE_T_FORMAT".", start_line);
  *error = "Unterminated command.";
  return true;
}

static bool find_symbol(lexer_T* lexer, dataList_T* tokens) {

  void add_simple_token(token_E token_type) {
    ((token_T*) dataList_add(tokens))->type = token_type;
  }

  switch (lexer->c) {
  case '{': add_simple_token(TOKEN_OPEN_CB);   break;
  case '}': add_simple_token(TOKEN_CLOSE_CB);  break;
  case '(': add_simple_token(TOKEN_OPEN_P);    break;
  case ')': add_simple_token(TOKEN_CLOSE_P);   break;
  /* case '+': add_simple_token(TOKEN_PLUS);      break; */
  /* case '-': add_simple_token(TOKEN_MINUS);     break; */
  /* case '*': add_simple_token(TOKEN_MULTIPLY);  break; */
  case ';': add_simple_token(TOKEN_SEMICOLON); break;
  default: return 0;
  }
  printf("symbol token: '%c'\n", lexer->c);
  forward(lexer);
  return 1;
}

static char* current_char_ptr(lexer_T* lexer) {
  return &(lexer->src.chars[lexer->i]);
}

void lexer_init(lexer_T* lexer, string_T src) {
  lexer->src = src;
  lexer->i = 0;
  lexer->line = 1;
  lexer->c = src.chars[0];
}

/* static bool is_identifier_allowed(char c) { */
/*   return // [a-z0-9/._-] */
/*     (c >= 'a' && c <= 'z') || */
/*     (c >= '0' && c <= '9') || */
/*     c == '.' || c == '_' || c == '-'; */
/* } */
