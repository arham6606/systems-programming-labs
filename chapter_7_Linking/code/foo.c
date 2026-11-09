// foo.c
#include <stdio.h>

static int counter = 0;       // local symbol (static global)
int shared = 42;              // global symbol (initialized → strong)
int uninit;                   // global symbol (uninitialized → weak)

static void helper(void) {    // local symbol (static function)
    counter++;
}

void foo(void) {              // global symbol (strong)
    helper();
    printf("hello %d\n", counter);
}
