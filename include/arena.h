#pragma once

#include "config.h"
#include "allocator.h"

COMMON_API Allocator make_arena_allocator(size_t size);
COMMON_API void free_arena_allocator(Allocator *alloc);
COMMON_API void arena_reset(Allocator *alloc);
COMMON_API size_t arena_mark(Allocator *alloc);
COMMON_API void arena_rewind(Allocator *alloc, size_t mark);

#define STATIC_ARENA_ALLOCATOR(NAME, SIZE)                         \
    static alignas(max_align_t) uint8_t NAME##_buffer[(SIZE)];    \
    static Arena NAME##_arena = {                                   \
        .base = NAME##_buffer,                                      \
        .size = (SIZE),                                             \
        .offset = 0                                                 \
    };                                                               \
    static Allocator NAME = {                                       \
        .alloc = arena_alloc,                                       \
        .free  = arena_free,                                        \
        .ctx   = &NAME##_arena                                      \
    }

/*
typedef struct {
    int x, y;
} Point;

int main(void) {
    Allocator A = make_arena_allocator(1024);

    Point *p1 = A.alloc(A.ctx, sizeof(Point));
    Point *p2 = A.alloc(A.ctx, sizeof(Point));

    p1->x = 1; p1->y = 2;
    p2->x = 3; p2->y = 4;

    arena_reset(&A); // free whole allocated

    Point *p3 = A.alloc(A.ctx, sizeof(Point));

    free_arena_allocator(&A);
    return 0;
}


STATIC_ARENA_ALLOCATOR(ast_alloc, 4096);

void example(void) {
    int *a = ast_alloc.alloc(ast_alloc.ctx, sizeof(int));
    int *b = ast_alloc.alloc(ast_alloc.ctx, sizeof(int));

    *a = 10;
    *b = 20;
}

*/
