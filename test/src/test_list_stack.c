#include <unity.h>
#include <common/allocator.h>
#include <common/list_stack.h>
#include "stack.h"

static Allocator allocator;

// .h
DECL_LIST_T(Struct1);
static Struct1List list;
DECL_LIST_STACK_T(Struct1);
static Struct1Stack list_stack;

// .c
IMPL_LIST_T(Struct1);
IMPL_LIST_STACK_T(Struct1);

void setUp(void) {
    allocator = sys_alloc;
    list = make_Struct1List(&allocator);
    list_stack = make_Struct1Stack(&list);
}

void tearDown(void) {
    list_free(&list);
}

TEST_FNS(list_)

int main(void) {
    UNITY_BEGIN();

    RUN_TEST_FNS(list_)

    return UNITY_END();
}
