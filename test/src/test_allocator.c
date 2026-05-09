#include <unity.h>
#include <stdlib.h>
#include <string.h>
#include <common/common.h>
#include <common/allocator.h>

// Mock allocator context for testing
typedef struct MockAlloc {
    Allocator base;
    size_t total_allocated;
    size_t allocation_count;
    size_t fail_after_count;  // Fail allocation after this many calls (0 = never fail)
    size_t current_count;
} MockAlloc;

static MockAlloc mockAlloc;
static AllocVTable vTable;
static Allocator *allocator;

// Mock allocation function
static void* mock_alloc(Allocator *a, size_t size) {
    MockAlloc *mock_ctx = container_of(a, MockAlloc, base);

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
static void mock_free(Allocator *a, void *ptr) {
    (void)a;  // Unused
    free(ptr);
}

void setUp(void) {
    vTable = (AllocVTable){
        .alloc = mock_alloc,
        .free = mock_free,
    };
    mockAlloc = (MockAlloc){
        .base = {
            .vTable = &vTable
        },
        .total_allocated = 0,
        .allocation_count = 0,
        .fail_after_count = 0, // Fail allocation after this many calls (0 = never fail)
        .current_count = 0
    };
    allocator = (Allocator *)&mockAlloc;
}

void tearDown(void) {
}

// Test basic allocation
void test_alloc_t_macro(void) {
    // Test allocating a struct
    typedef struct {
        int x;
        char y;
    } TestStruct;

    TestStruct *ptr = ALLOC_T(allocator, TestStruct);
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(0, mockAlloc.total_allocated - sizeof(TestStruct));

    FREE(allocator, ptr);
}

// Test zero allocation
void test_alloc_zero_macro(void) {
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
void test_alloc_zero_t_macro(void) {
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
void test_alloc_n_macro(void) {
    const int count = 10;
    int *arr = ALLOC_N(allocator, int, count);
    TEST_ASSERT_NOT_NULL(arr);

    // Verify we can access all elements
    for (int i = 0; i < count; i++) {
        arr[i] = i * 2;
        TEST_ASSERT_EQUAL(i * 2, arr[i]);
    }

    FREE(allocator, arr);
}

// Test zero array allocation
void test_alloc_zero_n_macro(void) {
    const int count = 16;
    int *arr = ALLOC_ZERO_N(allocator, int, count);
    TEST_ASSERT_NOT_NULL(arr);

    // Verify all elements are zero-initialized
    for (int i = 0; i < count; i++) {
        TEST_ASSERT_EQUAL(0, arr[i]);
    }

    // Verify we can write to all elements
    for (int i = 0; i < count; i++) {
        arr[i] = i + 1;
        TEST_ASSERT_EQUAL(i + 1, arr[i]);
    }

    FREE(allocator, arr);
}

// Test alloc_printf function
void test_alloc_printf(void) {
    const char *expected = "Hello, World! Value: 42";
    char *result = alloc_printf(allocator, "Hello, %s! Value: %d", "World", 42);

    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING(expected, result);

    FREE(allocator, result);
}

// Test allocation failure
void test_allocation_failure(void) {
    mockAlloc.fail_after_count = 1;  // Fail after 1 allocation

    // First allocation should succeed
    int *ptr1 = ALLOC_T(allocator, int);
    TEST_ASSERT_NOT_NULL(ptr1);
    TEST_ASSERT_EQUAL(1, mockAlloc.allocation_count);

    // Second allocation should fail
    int *ptr2 = ALLOC_T(allocator, int);
    TEST_ASSERT_NULL(ptr2);

    FREE(allocator, ptr1);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_alloc_t_macro);
    RUN_TEST(test_alloc_zero_macro);
    RUN_TEST(test_alloc_zero_t_macro);
    RUN_TEST(test_alloc_n_macro);
    RUN_TEST(test_alloc_zero_n_macro);
    RUN_TEST(test_alloc_printf);
    RUN_TEST(test_allocation_failure);

    return UNITY_END();
}
