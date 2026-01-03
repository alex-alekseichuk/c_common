#pragma once

#include <stddef.h>
#include <string.h>
#include "config.h"

_BEGIN_EXTERN_C

typedef void* (*alloc_fn)(void *ctx, size_t size);
typedef void  (*free_fn)(void *ctx, void *ptr);

typedef struct Allocator {
    alloc_fn alloc;
    free_fn  free;
    void    *ctx;
} Allocator;

// Node *n = alloc_t(&a, Node);
#define alloc_t(a, T) ((T*)(a)->alloc((a)->ctx, sizeof(T)))

#define alloc_n(a, T, n) ((T*)(a)->alloc((a)->ctx, sizeof(T) * (n)))

static inline void*
alloc_zero_impl(Allocator *a, size_t size) {
    void *p = a->alloc(a->ctx, size);
    if (p) memset(p, 0, size);
    return p;
}

#define alloc_zero(a, size) alloc_zero_impl((a), (size))

// Node *n = alloc_zero_t(&alloc, Node);
#define alloc_zero_t(a, T) \
    ((T*)alloc_zero((a), sizeof(T)))

// int *arr = alloc_zero_n(&alloc, int, 128);
#define alloc_zero_n(a, T, n) \
    ((T*)alloc_zero((a), sizeof(T) * (n)))

extern char* alloc_printf(Allocator *a, const char *fmt, ...);

_END_EXTERN_C

/*
If the output parameters are fixes/known, then you can use snprintf:

static char*
alloc_print_example(Allocator *A, const char *fmt,
               int x, const char *name)
{
    int n = snprintf(NULL, 0, fmt, x, name);
    if (n < 0) return NULL;

    char *buf = A->alloc(A->ctx, (size_t)n + 1);
    if (!buf) return NULL;

    snprintf(buf, (size_t)n + 1, fmt, x, name);
    return buf;
}
*/
