#pragma once

#include <stddef.h>
#include "config.h"
#include "allocator.h"
/*
| Task                       | Solution        |
| -------------------------- | --------------- |
| regular dynamic array      | `ARRAY`         |
| API / slice                | `ARRAY_VIEW`    |
| arena + large data         | `ARRAY_BUILDER` |
| small arrays               | `ARRAY_SSO`     |
*/

#define ARRAY(T, Name)                                                     \
typedef struct {                                                           \
    T      *data;                                                          \
    size_t  len;                                                           \
    size_t  size;                                                          \
    Allocator *A;                                                          \
} Name;                                                                    \
                                                                           \
static Name Name##_make(Allocator *A) {                                    \
    return (Name){ .A = A };                                               \
}                                                                          \
                                                                           \
static int Name##_reserve(Name *a, size_t new_size) {                     \
    if (new_size <= a->size) return 1;                                     \
    T *p = a->A->alloc(a->A->ctx, new_size * sizeof(T));                  \
    if (!p) return 0;                                                      \
    for (size_t i = 0; i < a->len; i++) p[i] = a->data[i];                \
    if (a->data) a->A->free(a->A->ctx, a->data);                          \
    a->data = p;                                                          \
    a->size = new_size;                                                   \
    return 1;                                                             \
}                                                                          \
                                                                           \
static int Name##_push(Name *a, T v) {                                     \
    if (a->len == a->size) {                                               \
        size_t ns = a->size ? a->size * 2 : 8;                            \
        if (!Name##_reserve(a, ns)) return 0;                             \
    }                                                                      \
    a->data[a->len++] = v;                                                 \
    return 1;                                                             \
}                                                                          \
                                                                           \
static void Name##_free(Name *a) {                                         \
    if (a->data) a->A->free(a->A->ctx, a->data);                          \
    *a = (Name){0};                                                       \
}



/*
✔ zero-allocation
✔ safe
✔ perfect for API

ARRAY_VIEW(int, IntView)
IntView v = { arr.data, arr.len };
*/
#define ARRAY_VIEW(T, Name)        \
typedef struct {                  \
    T      *data;                 \
    size_t  len;                  \
} Name;


/*
arena
large volumes
streaming / parsing
zero-copy
*/
#define ARRAY_BUILDER(T, Name, CHUNK)                                     \
typedef struct Chunk_##Name {                                             \
    struct Chunk_##Name *next;                                            \
    size_t len;                                                          \
    T data[CHUNK];                                                       \
} Chunk_##Name;                                                          \
                                                                          \
typedef struct {                                                         \
    Chunk_##Name *head;                                                  \
    Chunk_##Name *tail;                                                  \
    size_t total_len;                                                    \
    Allocator *A;                                                        \
} Name;                                                                  \
                                                                          \
static Name Name##_make(Allocator *A) {                                  \
    return (Name){ .A = A };                                             \
}                                                                        \
                                                                          \
static int Name##_push(Name *b, T v) {                                   \
    if (!b->tail || b->tail->len == CHUNK) {                             \
        Chunk_##Name *c = b->A->alloc(b->A->ctx, sizeof(*c));            \
        if (!c) return 0;                                                \
        c->len = 0; c->next = NULL;                                      \
        if (b->tail) b->tail->next = c;                                  \
        else b->head = c;                                                \
        b->tail = c;                                                     \
    }                                                                    \
    b->tail->data[b->tail->len++] = v;                                   \
    b->total_len++;                                                      \
    return 1;                                                            \
}



/*
✔ no allocations for small arrays
✔ perfect for strings, tokens, AST
*/
#define ARRAY_SSO(T, Name, N)                                             \
typedef struct {                                                          \
    size_t len, size;                                                     \
    Allocator *A;                                                         \
    T *data;                                                              \
    T small[N];                                                           \
} Name;                                                                   \
                                                                          \
static Name Name##_make(Allocator *A) {                                   \
    return (Name){ .A = A, .data = NULL };                                \
}                                                                         \
                                                                          \
static int Name##_push(Name *a, T v) {                                    \
    if (!a->data && a->len < N) {                                         \
        a->small[a->len++] = v;                                           \
        return 1;                                                         \
    }                                                                     \
    if (!a->data) {                                                       \
        a->size = N * 2;                                                  \
        a->data = a->A->alloc(a->A->ctx, a->size * sizeof(T));           \
        if (!a->data) return 0;                                           \
        for (size_t i = 0; i < a->len; i++)                               \
            a->data[i] = a->small[i];                                    \
    }                                                                     \
    if (a->len == a->size) {                                              \
        size_t ns = a->size * 2;                                          \
        T *p = a->A->alloc(a->A->ctx, ns * sizeof(T));                   \
        if (!p) return 0;                                                 \
        for (size_t i = 0; i < a->len; i++) p[i] = a->data[i];            \
        a->A->free(a->A->ctx, a->data);                                  \
        a->data = p; a->size = ns;                                       \
    }                                                                     \
    a->data[a->len++] = v;                                                \
    return 1;                                                            \
}

