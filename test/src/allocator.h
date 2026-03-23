#include <common/allocator.h>

// Test basic allocation
void testfn_alloc_t_macro(Allocator *allocator);

// Test zero allocation
void testfn_alloc_zero_macro(Allocator *allocator);

// Test zero allocation with types
void testfn_alloc_zero_t_macro(Allocator *allocator);

// Test array allocation
void testfn_alloc_n_macro(Allocator *allocator);

// Test zero array allocation
void testfn_alloc_zero_n_macro(Allocator *allocator);

// Test alloc_printf function
void testfn_alloc_printf(Allocator *allocator);

#define TEST_FN(Prefix, Name) void Prefix##Name(void) { testfn_##Name(&(Prefix##allocator)); }

#define TEST_FNS(Prefix) \
    TEST_FN(Prefix, alloc_t_macro); \
    TEST_FN(Prefix, alloc_zero_macro); \
    TEST_FN(Prefix, alloc_zero_t_macro); \
    TEST_FN(Prefix, alloc_n_macro); \
    TEST_FN(Prefix, alloc_zero_n_macro); \
    TEST_FN(Prefix, alloc_printf);

#define RUN_TEST_FN(Prefix, Name) RUN_TEST(Prefix##Name);

#define RUN_TEST_FNS(Prefix) \
    RUN_TEST_FN(Prefix, alloc_t_macro); \
    RUN_TEST_FN(Prefix, alloc_zero_macro); \
    RUN_TEST_FN(Prefix, alloc_zero_t_macro); \
    RUN_TEST_FN(Prefix, alloc_n_macro); \
    RUN_TEST_FN(Prefix, alloc_zero_n_macro); \
    RUN_TEST_FN(Prefix, alloc_printf);

