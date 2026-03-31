#pragma once
/**
 * Allocator interface for memory allocation.
 * It's used in data structures.
 * It has implementations:
 *   - malloc_allocator type uses system malloc/free/realloc
 *   - arena_allocator type uses pre-allocated buffer.
 *   - dummy_allocator instance is safe to be used with static memory; it doesn't really allocate.
 */

#include <stddef.h>
#include <stdalign.h>
#include <string.h>
#include "common/common.h"

_BEGIN_EXTERN_C

extern const size_t SIZE_T; // 8
extern const size_t ALIGN; // 32

#define BUFFER(NAME, T, SIZE) alignas(max_align_t) T NAME[(SIZE)];

typedef void* (*alloc_fn)(void *ctx, size_t size);
typedef void* (*realloc_fn)(void *ctx, void *ptr, size_t size);
typedef void (*free_fn)(void *ctx, void *ptr);
typedef size_t (*sizeof_fn)(void *ctx, void *ptr);

// allocator is an entity that can allocate and free memory
// it has abstract context implementation
typedef struct Allocator {
    alloc_fn alloc;
    realloc_fn realloc;
    free_fn  free;
    sizeof_fn _sizeof;
    void    *ctx;
} Allocator;

extern Allocator dummy_allocator;

#define ALLOC(a, size) ((a)->alloc((a)->ctx, (size)))

// allocate a struct
// Node *n = ALLOC_T(&a, Node);
#define ALLOC_T(a, T) ((T*)(a)->alloc((a)->ctx, sizeof(T)))

// allocate an array of n elements
#define ALLOC_N(a, T, n) ((T*)(a)->alloc((a)->ctx, sizeof(T) * (n)))

static inline void* alloc_zero(Allocator *a, size_t size) {
    void *p = a->alloc(a->ctx, size);
    if (p) memset(p, 0, size);
    return p;
}

// allocate severtal bytes and zero the memory
#define ALLOC_ZERO(a, size) alloc_zero((a), (size))

// allocate a struct and zero the memory
// Node *n = ALLOC_ZERO_T(&alloc, Node);
#define ALLOC_ZERO_T(a, T) \
    ((T*)alloc_zero((a), sizeof(T)))

// allocate an array of n elements and zero the memory
// int *arr = ALLOC_ZERO_N(&alloc, int, 128);
#define ALLOC_ZERO_N(a, T, n) \
    ((T*)alloc_zero((a), sizeof(T) * (n)))

#define REALLOC(a, ptr, size) (a)->realloc((a)->ctx, (ptr), (size))
#define REALLOC_N(a, ptr, T, n) ((T*)(a)->realloc((a)->ctx, (ptr), sizeof(T) * (n)))

// free memory
#define FREE(a, ptr) (a)->free((a)->ctx, (ptr))

#define SIZEOF(a, ptr) (a)->_sizeof((a)->ctx, (ptr))

// allocate a string and format it
COMMON_API char* alloc_printf(Allocator *a, const char *fmt, ...);

_END_EXTERN_C
