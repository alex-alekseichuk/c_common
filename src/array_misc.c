/*
| Task                       | Solution        |
| -------------------------- | --------------- |
| regular dynamic array      | `ARRAY`         |
| API / slice                | `ARRAY_VIEW`    |
| arena + large data         | `ARRAY_BUILDER` |
| small arrays               | `ARRAY_SSO`     |
*/

#define DEFINE_ARRAY_NAME(T, Name)                                        \
typedef struct Name  Name;                                                \
Name make_Name##_n(Allocator *alloc, size_t size);                        \
Name make_##Name(Allocator *alloc);                                       \
Name *alloc_Name##_n(Allocator *alloc, size_t size);                      \
Name *alloc_##Name(Allocator *alloc);                                     \
int Name##_reserve(Name *a, size_t new_size);                             \

#define DEFINE_ARRAY(T) DEFINE_ARRAY_NAME(T, T##Array)

#define ARRAY_NAME(T, Name)                                               \
typedef struct Name {                                                     \
    size_t size;                                                          \
    size_t len;                                                           \
    Allocator *alloc;                                                     \
    T *data;                                                              \
} Name;                                                                   \
                                                                          \
Name make_Name##_n(Allocator *alloc, size_t size) {                       \
    return (Name){                                                        \
        .size = size,                                                     \
        .len = 0,                                                         \
        .alloc = alloc                                                    \
        .data = ALLOC(alloc, size * sizeof(T))                            \
    };                                                                    \
}                                                                         \
                                                                          \
Name make_##Name(Allocator *alloc) {                                      \
    return make_Name##_n(alloc, 32);                                      \
}                                                                         \
                                                                          \
Name *alloc_Name##_n(Allocator *alloc, size_t size) {                     \
    Name *a = ALLOC_T(alloc, Name);                                       \
    if (!a) {                                                             \
        return NULL;                                                      \
    }                                                                     \
                                                                          \
    a->size = size;                                                       \
    a->len = 0;                                                           \
    a->alloc = alloc;                                                     \
    a->data = ALLOC(alloc, size * sizeof(T);                              \
    return a;                                                             \
}                                                                         \
                                                                          \
Name *alloc_##Name(Allocator *alloc) {                                    \
    return alloc_Name##_n(alloc, 32);                                     \
}                                                                         \
                                                                          \
void free_##Name(Name *a) {                                               \
    Name##_free(a);                                                       \
    FREE(a->alloc, a);                                                    \
}                                                                         \
                                                                          \
int Name##_reserve(Name *a, size_t new_size) {                            \
    if (new_size <= a->size) return 1;                                    \
    T *p = ALLOC_N(a->alloc, T, new_size);                                \
    if (!p) return 0;                                                     \
    memcpy(p, a->data, a->len * sizeof(T));                               \
    if (a->data) FREE(a->alloc, a->data);                                 \
    a->data = p;                                                          \
    a->size = new_size;                                                   \
    return 1;                                                             \
}                                                                         \
                                                                          \
int Name##_push(Name *a, T v) {                                           \
    if (a->len == a->size) {                                              \
        size_t ns = a->size ? a->size * 2 : 32;                           \
        if (!Name##_reserve(a, ns)) {                                     \
            return 0;                                                     \
        }                                                                 \
    }                                                                     \
    a->data[a->len++] = v;                                                \
                                                                          \
    return 1;                                                             \
}                                                                         \
                                                                          \
void Name##_free(Name *a) {                                               \
    if (a->data) FREE(a->alloc, a->data);                                 \
    a->size = 0;                                                          \
    a->len = 0;                                                           \
}

#define ARRAY(T) ARRAY_NAME(T, T##Array)


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

