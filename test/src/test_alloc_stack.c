#include <unity.h>
#include <common/malloc.h>
#include <common/alloc_stack.h>

static Allocator allocator;

void setUp(void) {
    allocator = make_malloc_allocator();
}

void tearDown(void) {
}

// struct1.h
typedef struct Struct1 Struct1;
DEFINE_ALLOC_STACK(Struct1);

// struct1.c
typedef struct Struct1 {
    const char *name;
    int age;
} Struct1;
IMPL_ALLOC_STACK(Struct1);

void test_struct_stack(void) {
    Struct1Stack stack = make_Struct1Stack(&allocator);

    Struct1 s, s0 = {0}, s1 = {.name="Alex", .age=17}, s2 = (Struct1){.name="Bill", .age=21};

    TEST_ASSERT_EQUAL(1, Struct1Stack_empty(&stack));
    s = Struct1Stack_top(&stack);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));
    s = Struct1Stack_pop(&stack);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));

    TEST_ASSERT_EQUAL(true, Struct1Stack_push(&stack, s1));
    TEST_ASSERT_EQUAL(0, Struct1Stack_empty(&stack));
    s = Struct1Stack_top(&stack);
    TEST_ASSERT_EQUAL_MEMORY(&s1, &s, sizeof(Struct1));
    TEST_ASSERT_EQUAL(true, Struct1Stack_push(&stack, s2));
    s = Struct1Stack_top(&stack);
    TEST_ASSERT_EQUAL_MEMORY(&s2, &s, sizeof(Struct1));
    s = Struct1Stack_pop(&stack);
    TEST_ASSERT_EQUAL_MEMORY(&s2, &s, sizeof(Struct1));
    s = Struct1Stack_pop(&stack);
    TEST_ASSERT_EQUAL_MEMORY(&s1, &s, sizeof(Struct1));
    
    TEST_ASSERT_EQUAL(1, Struct1Stack_empty(&stack));
    s = Struct1Stack_top(&stack);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));
    s = Struct1Stack_pop(&stack);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));

    Struct1Stack_free(&stack);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_struct_stack);

    return UNITY_END();
}
