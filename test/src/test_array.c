#include <unity.h>
#include <stdbool.h>
#include <common/arena_alloc.h>
#include <common/array.h>

static Allocator dynamic_allocator;

// .h
typedef struct Struct1 {
    int x;
    int y;
} Struct1;
DECL_ARRAY_T(Struct1)

// .c
#include <common/array_impl.h>
IMPL_ARRAY_T(Struct1)

static BUFFER(buffer, Struct1, 10);

static Array static_array;
static Array static_len_array;
static Array dynamic_array;

void globalSetUp(void) {
    dynamic_allocator = make_malloc_arena(10240);
    static_array = static_Struct1Array(buffer, 10);
    static_len_array = static_Struct1Array_len(buffer, 10, 0);
}
void globalTearDown(void) {
    arena_free(&dynamic_allocator);
}
void setUp(void) {
    dynamic_array = make_Struct1Array(&dynamic_allocator, 10);
}

void tearDown(void) {
    array_free(&dynamic_array);
}

void test_put_outside_array() {
    Struct1 s, s0 = {0}, s1 = {.x=1, .y=2};
    s = Struct1Array_get(&dynamic_array, 15);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));    
    int result = Struct1Array_put(&dynamic_array, 15, s1);
    TEST_ASSERT_EQUAL(1, result);
    s = Struct1Array_get(&dynamic_array, 15);
    TEST_ASSERT_EQUAL_MEMORY(&s1, &s, sizeof(Struct1));    
    s = Struct1Array_get(&dynamic_array, 14);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));    
}

void test_push_outside_array() {
    Struct1 s, s0 = {0}, s1 = {.x=1, .y=2};
    TEST_ASSERT_EQUAL(10, array_size(&dynamic_array));
    TEST_ASSERT_EQUAL(0, array_len(&dynamic_array));
    int result;
    for (int i = 0; i < 11; i++) {
        result = Struct1Array_push(&dynamic_array, s1);
        TEST_ASSERT_EQUAL(1, result);
    }
    TEST_ASSERT_EQUAL(20, array_size(&dynamic_array));
    TEST_ASSERT_EQUAL(11, array_len(&dynamic_array));
}

void test_array_of_struct(Array *array) {
    ((Struct1 *)array->data)[0].x = 1;
    ((Struct1 *)array->data)[0].y = 2;
}

void test_as_stack(Array *array) {
    Struct1 s, s0 = {0}, s1 = {.x=1, .y=2}, s2 = {.x=3, .y=4};

    TEST_ASSERT_EQUAL(1, array_empty(array));
    s = Struct1Array_top(array);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));
    s = Struct1Array_pop(array);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));

    TEST_ASSERT_EQUAL(true, Struct1Array_push(array, s1));
    TEST_ASSERT_EQUAL(0, array_empty(array));
    s = Struct1Array_top(array);
    TEST_ASSERT_EQUAL_MEMORY(&s1, &s, sizeof(Struct1));
    TEST_ASSERT_EQUAL(true, Struct1Array_push(array, s2));
    s = Struct1Array_top(array);
    TEST_ASSERT_EQUAL_MEMORY(&s2, &s, sizeof(Struct1));
    s = Struct1Array_pop(array);
    TEST_ASSERT_EQUAL_MEMORY(&s2, &s, sizeof(Struct1));
    s = Struct1Array_pop(array);
    TEST_ASSERT_EQUAL_MEMORY(&s1, &s, sizeof(Struct1));
    
    TEST_ASSERT_EQUAL(1, array_empty(array));
    s = Struct1Array_top(array);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));
    s = Struct1Array_pop(array);
    TEST_ASSERT_EQUAL_MEMORY(&s0, &s, sizeof(Struct1));
}

#define TEST_FN(Prefix, Name) void Prefix##Name(void) { test_##Name(&(Prefix##array)); }
#define TEST_FNS(Prefix) \
    TEST_FN(Prefix, array_of_struct) \
    TEST_FN(Prefix, as_stack) \

#define RUN_TEST_FN(Prefix, Name) RUN_TEST(Prefix##Name);
#define RUN_TEST_FNS(Prefix) \
    RUN_TEST_FN(Prefix, array_of_struct) \
    RUN_TEST_FN(Prefix, as_stack) \

TEST_FNS(static_)
TEST_FNS(static_len_)
TEST_FNS(dynamic_)

#undef TEST_FNS
#undef TEST_FN

int main(void) {
    globalSetUp();

    UNITY_BEGIN();

    RUN_TEST_FNS(static_)
    RUN_TEST_FNS(static_len_)
    RUN_TEST_FNS(dynamic_)

    RUN_TEST(test_put_outside_array);
    RUN_TEST(test_push_outside_array);

    #undef RUN_TEST_FNS
    #undef RUN_TEST_FN
    
    int result = UNITY_END();
    globalTearDown();

    return result;
}
