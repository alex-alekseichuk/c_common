#include <unity.h>
#include <common/allocator.h>
#include <common/array.h>
#include <common/array_stack.h>
#include "stack.h"

static Allocator *_allocator;

// .h
DECL_ARRAY_T(Struct1);
DECL_ARRAY_STACK_T(Struct1);

static ArrayStruct1Stack _array_stack;
static Struct1Stack *array_stack;

// .c
#include <common/array_impl.h>
IMPL_ARRAY_T(Struct1);
static Array array;
IMPL_ARRAY_STACK_T(Struct1);
void setUp(void) {
    _allocator = &sys_alloc;
    array = make_Struct1Array(_allocator, 1024);
    _array_stack = make_ArrayStruct1Stack(&array);
    array_stack = (Struct1Stack *)&_array_stack;
}

void tearDown(void) {
    array_free(&array);
}

TEST_FNS(array_)

int main(void) {
    UNITY_BEGIN();

    RUN_TEST_FNS(array_)

    return UNITY_END();
}
