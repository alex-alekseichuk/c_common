#include <complex.h>
#include <unity.h>
#include <common/arena.h>
#include "allocator.h"

static Allocator dynamic_allocator;
static Allocator static_allocator;
STATIC_ARENA(static_arena, 1024)

void globalSetUp(void) {
    dynamic_allocator = make_arena_allocator(1024);
    static_allocator = static_arena_allocator(&static_arena);
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
