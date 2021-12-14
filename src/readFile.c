#include "readFile.h"
#include "safe_allocation.h"

// allocates space for the string it returns, remember to free!
string_T file_to_string(const char* fileName, char** error) {

  string_T content = {0};
  
  // "rb" Read Bytes. "Bytes" is to prevent the reading operation from
  // automatically turning \r\n into \n, which would make the file length incorrect
  FILE* f = fopen(fileName, "rb"); 
  
  if (f == NULL) {
     *error =  "Couldn't open file.";
     return content;
  }
  
  content.len = file_size(f);
  content.chars = safe_malloc((content.len+1) * sizeof(char));
  // allocating len+1 for a '\0' on the end.
  // This is for printing purposes, if the string is
  // ever passed to printf then it won't fail or go into
  // unallocated memory.
  
  if (content.chars == NULL) {
    *error = "Couldn't allocate space for the file.";
    fclose(f);
    return content;
  }
  
  size_t bytes_read = fread(content.chars, 1, content.len, f);
  fclose(f); // Never forget
  
  content.chars[content.len] = '\0';

  if (bytes_read != content.len) {
    *error = "Couldn't read all bytes in file.";
    return content;
  }

  *error = 0;
  return content;
}


size_t file_size(FILE* file) {
  long int initialPos = ftell(file); // remember initial pos
  fseek(file, 0L, SEEK_END);         // goto end
  long int size = ftell(file);       // get index of end
  fseek(file, initialPos, SEEK_SET); // goto initial pos
  return (size_t)size;               // return index of end
}
