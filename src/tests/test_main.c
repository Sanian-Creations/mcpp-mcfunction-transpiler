#include "../main.c"
#include "sanian_unit.h"

// TODO write some tests for datalist, similar to pointerList;
// TODO write some tests for literally everything else, I've been ignoring these tests, haha

/* static void test_pointerList_0capacity() { */
/*   TEST_START; */

/*   // arrange */
/*   pointerList_T list; */
/*   list.arr = (void*)0xFFFFFFFF; // some non-null pointer */

/*   // act */
/*   pointerList_init(&list, 0); */
/*   void* actual_arr = list.arr; */
/*   pointerList_dispose(&list); */
  
/*   // assert */
/*   if (actual_arr != NULL) { */
/*     FAIL("pointerList_init: 0 capacity should set 'arr' to NULL"); */
/*   } */
/* } */

/* static void test_pointerList_add_one_item() { */
/*   TEST_START; */

/*   // arrange */
/*   int kek = 101; */
/*   pointerList_T list; */
/*   pointerList_init(&list, 2); */

/*   // act */
/*   pointerList_add(&list, &kek); */

/*   // assert */
/*   if (list.arr == NULL) { */
/*     pointerList_dispose(&list); */
/*     FAIL("pointerList_add: list.arr shouldn't be NULL after inserting one item"); */
/*   } */
/*   if (list.arr[0] != &kek) { */
/*     pointerList_dispose(&list); */
/*     FAIL("pointerList_add: the added value should be in the array"); */
/*   } */
/*   if (list.len != 1) { */
/*     pointerList_dispose(&list); */
/*     FAIL("pointerList_add: len should be 1 after adding a single value"); */
/*   } */
/*   pointerList_dispose(&list); */
/* } */

/* static void test_pointerList_add_shouldResize(size_t initialCapacity) { */
/*   TEST_START; */

/*   // arrange */
/*   size_t expected = initialCapacity * 2; */
/*   pointerList_T list; */
/*   pointerList_init(&list, initialCapacity); */
/*   list.len = initialCapacity; */
/*   int nice = 69; */

/*   // act */
/*   pointerList_add(&list, &nice); */
/*   size_t actual = list.capacity; */
/*   pointerList_dispose(&list); // no leaks! */

/*   // assert */
/*   if (actual != expected) { */
/*     FAIL("pointerList_add: Capacity should be %"SIZE_T_FORMAT" after resizing from %"SIZE_T_FORMAT", but was", initialCapacity*2, initialCapacity); */
/*   } */
/* } */

/* static void test_pointerList_cases() { */
/*   test_pointerList_0capacity(); */
/*   test_pointerList_add_one_item(); */
/*   test_pointerList_add_shouldResize(2); */
/*   test_pointerList_add_shouldResize(5); */
/*   test_pointerList_add_shouldResize(69); */
/* } */


/* static void test_string_match(char* arg1, char* arg2, bool expected) { */
/*   TEST_START; */

/*   // arrange */
/*   string_T str1, str2; */
/*   string_init(&str1, arg1); */
/*   string_init(&str2, arg2); */

/*   // act */
/*   bool result = string_match(&str1, &str2); */

/*   // assert */
/*   if (result != expected) { */
/*     FAIL("string_match: |%s| should %sbe equal to |%s|\n", arg1, expected ? "" : "not ", arg2); */
/*   } */
/* } */

/* static void test_string_match_cases(){ */
/*   test_string_match("",     "",    true); */
/*   test_string_match("yo",   "yo",  true); */
/*   test_string_match("abc",  "bca", false); */
/*   test_string_match("abc",  "abd", false); */
/*   test_string_match("aaaa", "aa",  false); */
/* } */


static void test_parseArgs(int argc, char** args, bool shouldSucceed) {
  TEST_START;
  // TODO once we have some flags, add tests to see if the options struct is as expected

  // arrange
  char* error;
  argOptions_T options = {0};

  // act
  parseArgs(argc, args, &options, &error);
  bool succeeded = error == 0;

  // assert
  if (succeeded != shouldSucceed){
    if (shouldSucceed) {
      FAIL("parseArgs: should succeed when given %d argument(s), instead it gave this error: \n\t%s", argc, error);
    } else {
      FAIL("parseArgs: should not succeed when given %d argument(s)", argc);
    }
  }
}

static void test_parseArgs_cases() {

  char* args[] = {"mcpp.exe", "someFile.txt", "-f", "-o", "outputName"};

  test_parseArgs(0, args, false); // too few args
  test_parseArgs(1, args, false); // too few args
  test_parseArgs(2, args, true);
  test_parseArgs(3, args, false); // too many args
}

int main() {
  test_parseArgs_cases();
  /* test_pointerList_cases(); */
  /* test_string_match_cases(); */
  
  printf("RESULT: [%d/%d] tests succeeded.\n", tests_run - tests_failed, tests_run);
  fflush(stdout);
}
