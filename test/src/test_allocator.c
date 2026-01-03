#include <unity.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"

// Mock allocator context for testing
typedef struct MockAllocContext {
    size_t total_allocated;
    size_t allocation_count;
    size_t fail_after_count;  // Fail allocation after this many calls (0 = never fail)
    size_t current_count;
} MockAllocContext;

// Mock allocation function
static void* mock_alloc(void *ctx, size_t size) {
    MockAllocContext *mock_ctx = (MockAllocContext*)ctx;

    if (mock_ctx->fail_after_count > 0 &&
        mock_ctx->current_count >= mock_ctx->fail_after_count) {
        return NULL;
    }

    void *ptr = malloc(size);
    if (ptr) {
        mock_ctx->total_allocated += size;
        mock_ctx->allocation_count++;
        mock_ctx->current_count++;
    }
    return ptr;
}

// Mock free function
static void mock_free(void *ctx, void *ptr) {
    (void)ctx;  // Unused
    free(ptr);
}

// Test setup function
void setUp(void) {
    // Called before each test
}

// Test teardown function
void tearDown(void) {
    // Called after each test
}

// Test basic allocation
void test_alloc_t_macro(void) {
    MockAllocContext ctx = {0, 0, 0, 0};
    Allocator allocator = {
        .alloc = mock_alloc,
        .free = mock_free,
        .ctx = &ctx
    };

    // Test allocating a struct
    typedef struct {
        int x;
        char y;
    } TestStruct;

    TestStruct *ptr = alloc_t(&allocator, TestStruct);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(0, ctx.total_allocated - sizeof(TestStruct));

    mock_free(&ctx, ptr);
}

// Test zero allocation
void test_alloc_zero_macro(void) {
    MockAllocContext ctx = {0, 0, 0, 0};
    Allocator allocator = {
        .alloc = mock_alloc,
        .free = mock_free,
        .ctx = &ctx
    };

    size_t size = 100;
    void *ptr = alloc_zero(&allocator, size);
    TEST_ASSERT_NOT_NULL(ptr);

    // Check that memory is zeroed
    char *bytes = (char*)ptr;
    for (size_t i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL(0, bytes[i]);
    }

    mock_free(&ctx, ptr);
}

// Test zero allocation with types
void test_alloc_zero_t_macro(void) {
    MockAllocContext ctx = {0, 0, 0, 0};
    Allocator allocator = {
        .alloc = mock_alloc,
        .free = mock_free,
        .ctx = &ctx
    };

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

    mock_free(&ctx, ptr);
}

// Test array allocation
void test_alloc_n_macro(void) {
    MockAllocContext ctx = {0, 0, 0, 0};
    Allocator allocator = {
        .alloc = mock_alloc,
        .free = mock_free,
        .ctx = &ctx
    };

    const int count = 10;
    int *arr = alloc_n(&allocator, int, count);
    TEST_ASSERT_NOT_NULL(arr);

    // Verify we can access all elements
    for (int i = 0; i < count; i++) {
        arr[i] = i * 2;
        TEST_ASSERT_EQUAL(i * 2, arr[i]);
    }

    mock_free(&ctx, arr);
}

// Test alloc_printf function
void test_alloc_printf(void) {
    MockAllocContext ctx = {0, 0, 0, 0};
    Allocator allocator = {
        .alloc = mock_alloc,
        .free = mock_free,
        .ctx = &ctx
    };

    const char *expected = "Hello, World! Value: 42";
    char *result = alloc_printf(&allocator, "Hello, %s! Value: %d", "World", 42);

    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(expected, result);

    mock_free(&ctx, result);
}

// Test allocation failure
void test_allocation_failure(void) {
    MockAllocContext ctx = {0, 0, 1, 0};  // Fail after 1 allocation
    Allocator allocator = {
        .alloc = mock_alloc,
        .free = mock_free,
        .ctx = &ctx
    };

    // First allocation should succeed
    int *ptr1 = alloc_t(&allocator, int);
    TEST_ASSERT_NOT_NULL(ptr1);
    TEST_ASSERT_EQUAL(1, ctx.allocation_count);

    // Second allocation should fail
    int *ptr2 = alloc_t(&allocator, int);
    TEST_ASSERT_NULL(ptr2);

    mock_free(&ctx, ptr1);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_alloc_t_macro);
    RUN_TEST(test_alloc_zero_macro);
    RUN_TEST(test_alloc_zero_t_macro);
    RUN_TEST(test_alloc_n_macro);
    RUN_TEST(test_alloc_printf);
    RUN_TEST(test_allocation_failure);

    return UNITY_END();
}
