#define SYNTAX_FILE
#include "syntax.h"

static void no_space_for(char* definition_type) {
  printf("[DEVELOPER_SUCKS_ERROR] Not enough space to store that many %s definitions, please modify the size of the %ss array\n", definition_type, definition_type);
}

// I could hardcode this with struct initializers but it's really just a pain to maintain that way.
void define_syntax() {
  define_comment("//", "\n", true);
  define_comment("/*", "*/", false);

  define_keyword("tick",      TOKEN_KEYW_TICK);
  define_keyword("load",      TOKEN_KEYW_LOAD);
  define_keyword("func",      TOKEN_KEYW_FUNCTION); // might make some regex-like thing where it allows "func(tion)?", not important enough to me now.
  define_keyword("function",  TOKEN_KEYW_FUNCTION);
  define_keyword("namespace", TOKEN_KEYW_NAMESPACE);
  define_keyword("if",        TOKEN_KEYW_IF);
  define_keyword("else",      TOKEN_KEYW_ELSE);
  define_keyword("while",     TOKEN_KEYW_WHILE);

  is_syntax_defined = true;
}

static void define_comment(char* start, char* end, bool eof_terminated) {
  if (comment_count >= comments_len) {
    no_space_for("comment");
    exit(-1);
  }
  string_init(&comments[comment_count].start, start);
  string_init(&comments[comment_count].end, end);
  comments[comment_count].eof_terminated = eof_terminated;
  comment_count++;
}

static void define_keyword(char* word, token_E token_type) {
  if (keyword_count >= keywords_len) {
    no_space_for("keyword");
    exit(-1);
  }
  string_init(&keywords[keyword_count].word, word);
  keywords[keyword_count].token_type = token_type; 
  keyword_count++;
}
