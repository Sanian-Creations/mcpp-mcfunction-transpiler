#include "lexer.h"



token_T* lex(string_T src, char** error) {
  
  lexer_T lexer;
  init(&lexer, src);
  


  

  *error = 0;
  return 0;
}

void init(lexer_T* lexer, string_T src) {
  lexer->i = 0;
  lexer->src = src;
  lexer->c = src.str[lexer->i];
}

// allocates space for the string it returns, remember to free!
string_T fileToStr(const char* fileName, char** error) {

  string_T content = {0};
  
  // "rb" Read Bytes. "Bytes" is to prevent the reading operation from
  // automatically turning \r\n into \n, which would make the file length incorrect
  FILE* f = fopen(fileName, "rb"); 
  
  if (f == NULL) {
     *error =  "Couldn't open file.";
     return content;
  }
  
  content.len = fileSize(f);
  content.str = malloc((content.len+1) * sizeof(char)); // +1 for '\0' on the end

  if (content.str == NULL) {
    *error = "Couldn't allocate space for the file.";
    fclose(f);
    return content;
  }
  
  size_t bytesRead = fread(content.str, 1, content.len, f);
  fclose(f); // Never forget
  
  content.str[content.len] = '\0';

  if (bytesRead != content.len) {
    *error = "Couldn't read all bytes in file.";
    return content;
  }

  *error = 0;
  return content;
}


size_t fileSize(FILE* file) {
  long int initialPos = ftell(file); // remember initial pos
  fseek(file, 0L, SEEK_END);         // goto end
  long int fileSize = ftell(file);   // get index of end
  fseek(file, initialPos, SEEK_SET); // goto initial pos
  return (size_t)fileSize;           // return index of end
}


bool strEq(const char* a, const char* b) {
  while (*a == *b && *a != '\0') {
    a++; b++;
  }
  return *a == *b;
}

