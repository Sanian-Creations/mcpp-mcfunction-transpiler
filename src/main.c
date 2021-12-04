#include "main.h"

#define EXIT_IF_ERROR(msg) do {	\
    if (msg != 0) { \
      printf("[!] %s\n", msg); \
      return -1; \
    } \
  } while (0)


int real_main(int argc, char** args) {

  printf("\n-----------------------\nRunning mc++ transpiler\n-----------------------\n\n");
  fflush(stdout);
  
  char* error;
  argOptions_T options = {0};

  parseArgs(argc, args, &options, &error);
  EXIT_IF_ERROR(error);

  string_T fileContent = fileToStr(options.file, &error); // allocates the string, remember to free!
  EXIT_IF_ERROR(error);

  dataList_T tokens;
  lex(fileContent, &tokens, &error);
  // DEBUG
  printf("Generated %"SIZE_T_FORMAT" tokens\n", tokens.len);
  for (size_t i = 0; i < tokens.len; i++) {
    printf("Token %u\n", ((token_T*)tokens.arr)[i].type);
  }
  // /DEBUG
  EXIT_IF_ERROR(error);
  
  
  // parse the tokens into intermediate representation
  //parse(tokens);
  
  // intermediate representation to output files
  
  
  free(fileContent.chars);
  dataList_dispose(&tokens);
  return 0;
}


void parseArgs(int argc, char** args, argOptions_T* options, char** error) {

  // No flags yet, the only argument should be the target file.
  // arg[0] is the name which was used to call this process, it is ignored.
  // arg[1] is the filename
  if (argc > 2) {
    *error = "Too many arguments.";
    return;
  }
  if (argc < 2) {
    *error = "Please provide a target file.";
    return;
  }

  options->file = args[1];
  *error = 0;
}
