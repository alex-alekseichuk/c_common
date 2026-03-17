#pragma once

#include <stddef.h>
#include <string.h>
#include <common/common.h>

_BEGIN_EXTERN_C

typedef void* (*alloc_fn)(void *ctx, size_t size);
typedef void  (*free_fn)(void *ctx, void *ptr);

// allocator is an entity that can allocate and free memory
// it has abstract context implementation
typedef struct Allocator {
    alloc_fn alloc;
    free_fn  free;
    void    *ctx;
} Allocator;

// allocate a struct
// Node *n = alloc_t(&a, Node);
#define alloc_t(a, T) ((T*)(a)->alloc((a)->ctx, sizeof(T)))

// allocate an array of n elements
#define alloc_n(a, T, n) ((T*)(a)->alloc((a)->ctx, sizeof(T) * (n)))

static inline void*
alloc_zero_impl(Allocator *a, size_t size) {
    void *p = a->alloc(a->ctx, size);
    if (p) memset(p, 0, size);
    return p;
}

// allocate severtal bytes and zero the memory
#define alloc_zero(a, size) alloc_zero_impl((a), (size))

// allocate a struct and zero the memory
// Node *n = alloc_zero_t(&alloc, Node);
#define alloc_zero_t(a, T) \
    ((T*)alloc_zero((a), sizeof(T)))

// allocate an array of n elements and zero the memory
// int *arr = alloc_zero_n(&alloc, int, 128);
#define alloc_zero_n(a, T, n) \
    ((T*)alloc_zero((a), sizeof(T) * (n)))

// free memory
#define alloc_free(a, ptr) (a)->free((a)->ctx, (ptr))

// allocate a string and format it
COMMON_API char* alloc_printf(Allocator *a, const char *fmt, ...);

_END_EXTERN_C
