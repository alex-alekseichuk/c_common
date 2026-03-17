#include <unity.h>
#include <stdlib.h>
#include <string.h>

#include <common/malloc.h>

static Allocator allocator;

// Test setup function
void setUp(void) {
    allocator = make_malloc_allocator();
}

// Test teardown function
void tearDown(void) {
    // Called after each test
}

// Test basic allocation
void test_alloc_t_macro(void) {
    // Test allocating a struct
    typedef struct {
        int x;
        char y;
    } TestStruct;

    TestStruct *ptr = alloc_t(&allocator, TestStruct);
    TEST_ASSERT_NOT_NULL(ptr);

    alloc_free(&allocator, ptr);
}

// Test zero allocation
void test_alloc_zero_macro(void) {
    size_t size = 100;
    void *ptr = alloc_zero(&allocator, size);
    TEST_ASSERT_NOT_NULL(ptr);

    // Check that memory is zeroed
    char *bytes = (char*)ptr;
    for (size_t i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL(0, bytes[i]);
    }

    alloc_free(&allocator, ptr);
}

// Test zero allocation with types
void test_alloc_zero_t_macro(void) {
    typedef struct {
        int x;
        char y;
        double z;
    } TestStruct;

    TestStruct *ptr = alloc_zero_t(&allocator, TestStruct);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(0, ptr->x);
    TEST_ASSERT_EQUAL(0, ptr->y);
    TEST_ASSERT_EQUAL(0.0, ptr->z);

    alloc_free(&allocator, ptr);
}

// Test array allocation
void test_alloc_n_macro(void) {
    const int count = 10;
    int *arr = alloc_n(&allocator, int, count);
    TEST_ASSERT_NOT_NULL(arr);

    alloc_free(&allocator, arr);
}

// Test zero array allocation
void test_alloc_zero_n_macro(void) {
    const int count = 16;
    int *arr = alloc_zero_n(&allocator, int, count);
    TEST_ASSERT_NOT_NULL(arr);

    // Verify all elements are zero-initialized
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_EQUAL(0, arr[i]);
    }

    alloc_free(&allocator, arr);
}

// Test alloc_printf function
void test_alloc_printf(void) {
    const char *expected = "Hello, World! Value: 42";
    char *result = alloc_printf(&allocator, "Hello, %s! Value: %d", "World", 42);

    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(expected, result);

    alloc_free(&allocator, result);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_alloc_t_macro);
    RUN_TEST(test_alloc_zero_macro);
    RUN_TEST(test_alloc_zero_t_macro);
    RUN_TEST(test_alloc_n_macro);
    RUN_TEST(test_alloc_zero_n_macro);
    RUN_TEST(test_alloc_printf);

    return UNITY_END();
}
