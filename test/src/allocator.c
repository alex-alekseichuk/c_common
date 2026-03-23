#include <unity.h>
#include <common/allocator.h>

// Test basic allocation
void testfn_alloc_t_macro(Allocator *allocator) {
    // Test allocating a struct
    typedef struct {
        int x;
        char y;
    } TestStruct;

    TestStruct *ptr = ALLOC_T(allocator, TestStruct);
    TEST_ASSERT_NOT_NULL(ptr);

    FREE(allocator, ptr);
}

// Test zero allocation
void testfn_alloc_zero_macro(Allocator *allocator) {
    size_t size = 100;
    void *ptr = alloc_zero(allocator, size);
    TEST_ASSERT_NOT_NULL(ptr);

    // Check that memory is zeroed
    char *bytes = (char*)ptr;
    for (size_t i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL(0, bytes[i]);
    }

    FREE(allocator, ptr);
}

// Test zero allocation with types
void testfn_alloc_zero_t_macro(Allocator *allocator) {
    typedef struct {
        int x;
        char y;
        double z;
    } TestStruct;

    TestStruct *ptr = ALLOC_ZERO_T(allocator, TestStruct);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(0, ptr->x);
    TEST_ASSERT_EQUAL(0, ptr->y);
    TEST_ASSERT_EQUAL(0.0, ptr->z);

    FREE(allocator, ptr);
}

// Test array allocation
void testfn_alloc_n_macro(Allocator *allocator) {
    const int count = 10;
    int *arr = ALLOC_N(allocator, int, count);
    TEST_ASSERT_NOT_NULL(arr);

    FREE(allocator, arr);
}

// Test zero array allocation
void testfn_alloc_zero_n_macro(Allocator *allocator) {
    const int count = 16;
    int *arr = ALLOC_ZERO_N(allocator, int, count);
    TEST_ASSERT_NOT_NULL(arr);

    // Verify all elements are zero-initialized
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_EQUAL(0, arr[i]);
    }

    FREE(allocator, arr);
}

// Test alloc_printf function
void testfn_alloc_printf(Allocator *allocator) {
    const char *expected = "Hello, World! Value: 42";
    char *result = alloc_printf(allocator, "Hello, %s! Value: %d", "World", 42);

    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(expected, result);

    FREE(allocator, result);
}
