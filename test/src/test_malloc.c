#include <unity.h>
#include <common/malloc.h>
#include "allocator.h"

static Allocator malloc_allocator;

void setUp(void) {
    malloc_allocator = make_malloc_allocator();
}

void tearDown(void) {
}

TEST_FNS(malloc_)

int main(void) {
    UNITY_BEGIN();

    RUN_TEST_FNS(malloc_)

    return UNITY_END();
}
