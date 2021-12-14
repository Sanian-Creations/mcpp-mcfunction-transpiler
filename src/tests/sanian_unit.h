#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int tests_run;
int tests_failed;

#define TEST_START tests_run++
#define FAIL(a, ...) do{tests_failed++;printf("[%d] "a"\n",tests_run,##__VA_ARGS__);return;}while(0)
