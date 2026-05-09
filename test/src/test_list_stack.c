#include <unity.h>
#include <common/allocator.h>
#include <common/list_stack.h>
#include "stack.h"

static Allocator *_allocator;

// .h
DECL_LIST_T(Struct1);
DECL_LIST_STACK_T(Struct1);
static ListStruct1Stack _list_stack;
static Struct1Stack *list_stack;

// .c
IMPL_LIST_T(Struct1);
static Struct1List _list;
IMPL_LIST_STACK_T(Struct1);

void setUp(void) {
    _allocator = &sys_alloc;
    _list = make_Struct1List(_allocator);
    _list_stack = make_Struct1Stack(&_list);
    list_stack = (Struct1Stack *)&_list_stack;
}

void tearDown(void) {
    list_free(&_list);
}

TEST_FNS(list_)

int main(void) {
    UNITY_BEGIN();

    RUN_TEST_FNS(list_)

    return UNITY_END();
}
