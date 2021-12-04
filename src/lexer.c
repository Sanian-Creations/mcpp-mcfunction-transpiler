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
  
  while (lexer.c != 0) {

    // 1. move past all whitespace
    skip_whitespace(&lexer);
    if (lexer.c == 0) break;
    
    // 2. move past comments
    if (skip_comments(&lexer, error)) {
      if (*error) return;
      continue;
    }
    
    // 3. keywords
    if (find_keyword(&lexer, tokens)) continue;
    
    // 4. strings
    if (find_string(&lexer, tokens, error)) {
      if (*error) return;
      continue;
    }
    
    // 5. raw blocks
    //  start:   @{
    //  content: anything
    //  end:     newline (optional whitespace) }@

    // 6. static symbols (operators, braces, etc.)
    
    printf("Unknown character '%c' (ASCII %d) on line %"SIZE_T_FORMAT"\n", lexer.c, (int)lexer.c, lexer.line);
    *error = "Unknown character.";
    return;
  } // end of while-loop
} // end of lex()


static void forward(lexer_T* lexer) {
  if (lexer->c == '\n') lexer->line++;
  
  if (++(lexer->i) < lexer->src.len) {
    lexer->c = lexer->src.chars[lexer->i];
  } else {
    lexer->c = 0;
  }
}

static void forward_by(lexer_T* lexer, size_t offset) {
  offset += lexer->i;
  if (offset >= lexer->src.len) {
    lexer->c = 0;
    return;
  }

  for (;lexer->i < offset; lexer->i++) {
    if (lexer->src.chars[lexer->i] == '\n') lexer->line++;
  }

  lexer->c = lexer->src.chars[lexer->i];
}

static char peek(lexer_T* lexer, size_t offset) {
  offset += lexer->i;
  if (offset >= lexer->src.len) return 0; // must be in-bounds
  
  return lexer->src.chars[offset];
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

static bool skip_until(lexer_T* lexer, string_T* str) {
  while (lexer->i < lexer->src.len) {
    if (match_here(lexer, str)) {
      forward_by(lexer, str->len); // move past it, too
      return true;
    }
    forward(lexer);
  }
  return false;
}

static void skip_whitespace(lexer_T* lexer) {
  // whitespace is ascii 9 10 11 12 13 and 32
  // 9 is '\t' and 13 is '\r', 32 is space
  while((lexer->c >= '\t' && lexer->c <= '\r') || lexer->c == ' ') {
    forward(lexer);
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

static void invalid_string(lexer_T* lexer, char** error) {
  *error = "Invalid string, missing endquote. Strings may not span multiple lines.";
  printf("\nInvalid string on line %"SIZE_T_FORMAT"\n", lexer->line);
}


static bool skip_comments(lexer_T* lexer, char** error) {
  size_t start = lexer->line;

  for (size_t i = 0; i < comments_len; i++) {
    
    if (!match_here(lexer, &comments[i].start)) continue; 

    forward_by(lexer, comments[i].start.len);

    // continue when:
    // a) Found the end symbol, or,
    // b) This comment may also end by reaching the eof
    if (skip_until(lexer, &comments[i].end) || comments[i].eof_terminated) {
      return true; // found a valid comment
    }
    
    // skip_until reached eof, and that isn't a valid way to terminate this comment.
    printf("Unterminated comment starting on line %"SIZE_T_FORMAT"\n", start);
    *error = "Unterminated comment.";
    
    return true; // found a comment, it din't end properly, an error was printed and we've reached eof.
  }
  
  return false; // no comment found.
}

static bool find_keyword(lexer_T* lexer, dataList_T* tokens) {
  if (!is_lowercase_letter(lexer->c)) return false; // found no word

  // mark the start of the word
  string_T word = {0};
  word.chars = &(lexer->src.chars[lexer->i]);

  // loop until end of word
  do {
    forward(lexer);
    word.len++;
  } while (is_lowercase_letter(lexer->c));

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
  if (lexer->c == 0) {
    invalid_string(lexer, error);
    return true;
  }
      
  // This string won't be 'processed' yet (no unescaping the characters)
  // simply mark the starting position and its length in the source string
  token_T* token = dataList_add(tokens);
  token->type = TOKEN_STRING;
  token->value.str.chars = &(lexer->src.chars[lexer->i]);
  token->value.str.len = 0;
      
  while (!(lexer->c == '"' && peek(lexer, -1) != '\\')) {
    if (lexer->c == '\n' || lexer->c == 0) {
      invalid_string(lexer, error);
      return true;
    }
    forward(lexer);
    token->value.str.len++;
  }
      
  forward(lexer); // move past the ending quote

  printf("string: ");
  print_string(&token->value.str);
  putchar('\n');
  return true;
}
