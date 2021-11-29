#define SYNTAX_FILE
#include "syntax.h"

void define_syntax() {
  define_comment("//", "\n");
  define_comment("/*", "*/");

  define_keyword("tick",      TOKEN_KEYW_TICK);
  define_keyword("load",      TOKEN_KEYW_LOAD);
  define_keyword("func",      TOKEN_KEYW_FUNCTION); // might make some regex-like thing where it allows "func(tion)?", not important enough to me now.
  define_keyword("function",  TOKEN_KEYW_FUNCTION);
  define_keyword("namespace", TOKEN_KEYW_NAMESPACE);

  is_syntax_defined = true;
}

static void define_comment(char* start, char* end) {
  if (comment_count >= comments_len) {
    printf("[DEVELOPER_SUCKS_ERROR] Not enough space to store that many comment definitions\n");
    exit(-1);
  }
  string_init(&comments[comment_count].start, start);
  string_init(&comments[comment_count].end, end); 
  comment_count++;
}

static void define_keyword(char* word, token_E token_type) {
  if (keyword_count >= keywords_len) {
    printf("[DEVELOPER_SUCKS_ERROR] Not enough space to store that many keyword definitions\n");
    exit(-1);
  }
  string_init(&keywords[keyword_count].word, word);
  keywords[keyword_count].token_type = token_type; 
  keyword_count++;
}
