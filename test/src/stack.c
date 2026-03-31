#include <unity.h>
#include <common/array_stack.h>
#include "stack.h"

void stackfn_struct_stack(Struct1Stack *stack) {
    Struct1 s, s0 = {0}, s1 = {.name="Alex", .age=17}, s2 = (Struct1){.name="Bill", .age=21};
    TEST_ASSERT_EQUAL(1, STACK_EMPTY(stack));

    s = STACK_TOP(stack);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));
    s = STACK_POP(stack);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));

    TEST_ASSERT_EQUAL(1, STACK_PUSH(stack, s1));
    TEST_ASSERT_EQUAL(0, STACK_EMPTY(stack));
    s = STACK_TOP(stack);
    TEST_ASSERT_EQUAL_MEMORY(&s1, &s, sizeof(Struct1));
    TEST_ASSERT_EQUAL(1, STACK_PUSH(stack, s2));
    s = STACK_TOP(stack);
    TEST_ASSERT_EQUAL_MEMORY(&s2, &s, sizeof(Struct1));
    s = STACK_POP(stack);
    TEST_ASSERT_EQUAL_MEMORY(&s2, &s, sizeof(Struct1));
    s = STACK_POP(stack);
    TEST_ASSERT_EQUAL_MEMORY(&s1, &s, sizeof(Struct1));
    
    TEST_ASSERT_EQUAL(1, STACK_EMPTY(stack));
    s = STACK_TOP(stack);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));
    s = STACK_POP(stack);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));
}
