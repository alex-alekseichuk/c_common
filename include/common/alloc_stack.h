#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "common/allocator.h"

_BEGIN_EXTERN_C

#define DEFINE_ALLOC_STACK_NAME(T, Name)                                  \
typedef struct Name Name;                                           \
COMMON_API Name make_stack_n(Allocator *alloc, size_t size);        \
COMMON_API Name make_stack(Allocator *alloc);                       \
COMMON_API void stack_free(Name *stack);                            \
COMMON_API int stack_push(Name *stack, T value);                    \
COMMON_API T stack_pop(Name *stack);                                \
COMMON_API T stack_top(Name *stack);                                \
COMMON_API int stack_empty(Name *stack);

#define DEFINE_ALLOC_STACK(T) DEFINE_ALLOC_STACK_NAME(T, T##Stack)

#define IMPL_ALLOC_STACK_NAME(T, Name)                                    \
typedef struct Name {                                               \
    size_t  size;                                                   \
    size_t  len;                                                    \
    Allocator *alloc;                                               \
    T *data;                                                        \
} Name;                                                             \
                                                                    \
Name make_Name##_n(Allocator *alloc, size_t size) {                 \
    return (Name){                                                  \
        .size = size,                                               \
        .len = 0,                                                   \
        .alloc = alloc,                                             \
        .data = ALLOC(alloc, size * sizeof(T))                      \
    };                                                              \
}                                                                   \
                                                                    \
Name make_##Name(Allocator *alloc) {                                \
    return make_Name##_n(alloc, 32);                                \
}                                                                   \
                                                                    \
void Name##_free(Name *stack) {                                     \
    if (stack->size == 0) {                                         \
        return;                                                     \
    }                                                               \
    FREE(stack->alloc, stack->data);                                \
    stack->size = 0;                                                \
    stack->len = 0;                                                 \
}                                                                   \
                                                                    \
int Name##_push(Name *stack, T value) {                             \
    if (stack->len >= stack->size) {                                \
        return 0;                                                   \
    }                                                               \
                                                                    \
    stack->len++;                                                   \
    stack->data[stack->len - 1] = value;                            \
                                                                    \
    return 1;                                                       \
}                                                                   \
                                                                    \
int Name##_empty(Name *stack) {                                     \
    return stack->len == 0;                                         \
}                                                                   \
                                                                    \
T Name##_pop(Name *stack) {                                         \
    if (stack->len == 0) {                                          \
        return (T){0};                                              \
    }                                                               \
                                                                    \
    stack->len--;                                                   \
    return stack->data[stack->len];                                 \
}                                                                   \
                                                                    \
T Name##_top(Name *stack) {                                         \
    if (stack->len == 0) {                                          \
        return (T){0};                                              \
    }                                                               \
                                                                    \
    return stack->data[stack->len - 1];                             \
}                                                                   \

#define IMPL_ALLOC_STACK(T) IMPL_ALLOC_STACK_NAME(T, T##Stack)

_END_EXTERN_C
