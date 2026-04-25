#include <unity.h>
#include <common/allocator.h>
#include "allocator.h"

static Allocator malloc_allocator;

void setUp(void) {
    malloc_allocator = sys_alloc;
}

void tearDown(void) {
}

TEST_FNS(malloc_)

int main(void) {
    UNITY_BEGIN();

    RUN_TEST_FNS(malloc_)

    return UNITY_END();
}
