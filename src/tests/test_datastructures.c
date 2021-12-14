#include "../macros.h"
#include "../datastructures.c"
#include "sanian_unit.h"

// TODO write some tests for datalist, similar to pointerList;
// TODO write some tests for literally everything else, I've been ignoring these tests, haha

static void test_pointerList_0capacity() {
  TEST_START;

  // arrange
  pointerList_T list;
  list.arr = (void*)0xFFFFFFFF; // some non-null pointer

  // act
  pointerList_init(&list, 0);
  void* actual_arr = list.arr;
  pointerList_dispose(&list);
  
  // assert
  if (actual_arr != NULL) {
    FAIL("pointerList_init: 0 capacity should set 'arr' to NULL");
  }
}

static void test_pointerList_add_one_item() {
  TEST_START;

  // arrange
  int kek = 101;
  pointerList_T list;
  pointerList_init(&list, 2);

  // act
  pointerList_add(&list, &kek);

  // assert
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

static void test_pointerList_add_shouldResize(size_t initialCapacity) {
  TEST_START;

  // arrange
  size_t expected = initialCapacity * 2;
  pointerList_T list;
  pointerList_init(&list, initialCapacity);
  list.len = initialCapacity;
  int nice = 69;

  // act
  pointerList_add(&list, &nice);
  size_t actual = list.capacity;
  pointerList_dispose(&list); // no leaks!

  // assert
  if (actual != expected) {
    FAIL("pointerList_add: Capacity should be %"SIZE_T_FORMAT" after resizing from %"SIZE_T_FORMAT", but was", initialCapacity*2, initialCapacity);
  }
}

static void test_pointerList_cases() {
  test_pointerList_0capacity();
  test_pointerList_add_one_item();
  test_pointerList_add_shouldResize(2);
  test_pointerList_add_shouldResize(5);
  test_pointerList_add_shouldResize(69);
}


int main() {
  test_pointerList_cases();
    
  printf("RESULT: [%d/%d] tests succeeded.\n", tests_run - tests_failed, tests_run);
  fflush(stdout);
}
