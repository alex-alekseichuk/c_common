#include <unity.h>
#include <common/allocator.h>
#include <common/array.h>
#include <common/array_stack.h>
#include "stack.h"

static Allocator allocator;

// .h
DECL_ARRAY_T(Struct1);
DECL_ARRAY_STACK_T(Struct1);

static Array array;
static Struct1Stack array_stack;

// .c
#include <common/array_impl.h>
void setUp(void) {
    allocator = sys_alloc;
    array = make_Struct1Array(&allocator, 1024);
    array_stack = make_Struct1Stack(&array);
}

void tearDown(void) {
    array_free(&array);
}

IMPL_ARRAY_T(Struct1);
IMPL_ARRAY_STACK_T(Struct1);

TEST_FNS(array_)

int main(void) {
    UNITY_BEGIN();

    RUN_TEST_FNS(array_)

    return UNITY_END();
}
