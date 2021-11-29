#include "../lexer.h"
#include "../main.h"
#include "../datastructures.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

int tests_run;
int tests_failed;

#define TEST_START tests_run++
#define FAIL(a, ...) do{tests_failed++;printf("[%d] "a"\n",tests_run,##__VA_ARGS__);return;}while(0)

// static keyword means this function is only available from within this file
// if it is #included elsewhere this function cannot be called.

// TODO write some tests for datalist, similar to pointerList;



static void test_pointerList_add_shouldResize(size_t initialCapacity) {
  TEST_START;
  
  int kek = 101;
  pointerList_T list;
  pointerList_init(&list, initialCapacity);
  list.len = initialCapacity;
  pointerList_add(&list, &kek);

  if (list.capacity != initialCapacity*2) {
    pointerList_dispose(&list);
    FAIL("pointerList_add: Capacity should be %d after resizing from %d", initialCapacity*2, initialCapacity);
  }
  pointerList_dispose(&list);
}

static void test_pointerList_add() {
  TEST_START;

  int kek = 101;
  pointerList_T list;
  pointerList_init(&list, 2);
  pointerList_add(&list, &kek);
  
  if (list.arr == NULL) {
    pointerList_dispose(&list);
    FAIL("pointerList_add: list.arr shouldn't be NULL after inserting one item");
  }
  if (list.arr[0] != &kek) {
    pointerList_dispose(&list);
    FAIL("pointerList_add: the added value should be in the array");
  }
  if (list.len != 1) {
    pointerList_dispose(&list);
    FAIL("pointerList_add: len should be 1 after adding a single value");
  }
  pointerList_dispose(&list);
}

static void test_pointerList_0capacity() {
  TEST_START;

  pointerList_T list;
  list.arr = (void*)0xFFFFFFFF; // some non-null pointer
  
  pointerList_init(&list, 0);
  
  if (list.arr != NULL) {
    pointerList_dispose(&list);
    FAIL("pointerList_init: 0 capacity should set 'arr' to NULL");
  }
  pointerList_dispose(&list);
}

static void test_pointerList() {
  test_pointerList_0capacity();
  test_pointerList_add();
  test_pointerList_add_shouldResize(2);
  test_pointerList_add_shouldResize(5);
  test_pointerList_add_shouldResize(69);
}


static void test_parseArgs_(int argc, char** args, bool shouldSucceed) {
  TEST_START;
  // TODO once we have some flags, add teststo see if the options struct is as expected

  char* error;
  argOptions_T options = {0};
  
  parseArgs(argc, args, &options, &error);
  bool succeeded = error == 0;

  if (succeeded != shouldSucceed){
    if (shouldSucceed) {
      FAIL("parseArgs: should succeed when given %d argument(s), instead it gave this error: \n\t%s", argc, error);
    } else { 
      FAIL("parseArgs: should not succeed when given %d argument(s)", argc);
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



int main() {
  test_parseArgs();
  test_pointerList();
  
  printf("RESULT: [%d/%d] tests succeeded.\n", tests_run - tests_failed, tests_run);
  fflush(stdout);
}
