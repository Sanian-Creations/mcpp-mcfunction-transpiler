#include "../lexer.h"
#include "../main.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

int tests_run;
int tests_failed;

#define TEST_START tests_run++
#define FAIL tests_failed++


// static keyword means this function is only available from within this file
// if it is #included elsewhere this function cannot be called.



static void test_parseArgs_(int argc, char** args, bool shouldSucceed) {
  TEST_START;
  // TODO once we have some flags, add teststo see if the options struct is as expected

  char* error;
  argOptions_T options = {0};
  
  parseArgs(argc, args, &options, &error);
  bool succeeded = error == 0;

  if (succeeded != shouldSucceed){
    FAIL;
    if (shouldSucceed) {
      fprintf(stdout, "parseArgs: should succeed when given %d argument(s), instead it gave this error: \n\t%s\n", argc, error);
    } else { 
      fprintf(stdout, "parseArgs: should not succeed when given %d argument(s)\n", argc);
    }
  }
}

static void test_parseArgs() {
  
  char* args[] = {"mcpp.exe", "someFile.txt", "-f", "-o", "outputName"};
  
  test_parseArgs_(0, args, false); // too few args
  test_parseArgs_(1, args, false); // too few args
  test_parseArgs_(2, args, true);
  test_parseArgs_(3, args, false); // too many args
}


static void test_strEq_(char* arg1, char* arg2, bool expected) {
  TEST_START;

  bool result = strEq(arg1, arg2);

  if (result != expected) {
    FAIL;
    fprintf(stdout, "strEq: |%s| should %sbe equal to |%s|\n", arg1, expected ? "" : "not ", arg2);
  }
}

static void test_strEq(){  
  test_strEq_("yo", "yo", true);
  test_strEq_("", "", true);
  test_strEq_("abc", "bca", false);
  test_strEq_("abc", "abd", false);
}


int main(int argc, char** args) {
  test_strEq();
  test_parseArgs();
  
  fflush(stdout);  
  fprintf(stderr, "RESULT: [%d/%d] tests succeeded.\n", tests_run - tests_failed, tests_run);
}
