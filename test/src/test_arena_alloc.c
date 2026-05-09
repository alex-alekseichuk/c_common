#include <complex.h>
#include <unity.h>
#include <common/common.h>
#include <common/arena_alloc.h>
#include "allocator.h"

static Arena *dynamic_arena;
static Allocator *dynamic_allocator;

static BUFFER(static_allocator_buffer, uint8_t, 10240);
static Arena *static_arena;
static Allocator *static_allocator;

void globalSetUp(void) {
    dynamic_arena = make_malloc_arena(10240);
    dynamic_allocator = (Allocator *)dynamic_arena;
    static_arena = make_buffer_arena(static_allocator_buffer, 10240);
    static_allocator = (Allocator *)static_arena;
}
void globalTearDown(void) {
    arena_free(dynamic_arena);
}
void setUp(void) {
}
void tearDown(void) {
}

TEST_FNS(dynamic_)
TEST_FNS(static_)

int main(void) {
    globalSetUp();

    UNITY_BEGIN();

    RUN_TEST_FNS(dynamic_)
    RUN_TEST_FNS(static_)

    int result = UNITY_END();
    globalTearDown();

    return result;
}
