#include <complex.h>
#include <unity.h>
#include <common/arena.h>
#include "allocator.h"

static Allocator dynamic_allocator;

static Allocator static_allocator;
static BUFFER(static_allocator_buffer, uint8_t, 10240);

void globalSetUp(void) {
    dynamic_allocator = make_malloc_arena(10240);
    static_allocator = make_buffer_arena(static_allocator_buffer, 10240);
}
void globalTearDown(void) {
    arena_free(&dynamic_allocator);
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
