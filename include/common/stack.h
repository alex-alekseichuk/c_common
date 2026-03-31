#pragma once
/**
 * Stack interface for a stack of type T.
 */

#include "common/common.h"

_BEGIN_EXTERN_C

#define DECL_STACK_T_NAME(T, Name) \
typedef int (*push_fn)(void *ctx, T value); \
typedef T (*pop_fn)(void *ctx); \
typedef T (*top_fn)(void *ctx); \
typedef int (*empty_fn)(void *ctx); \
 \
typedef struct Name { \
    push_fn push; \
    pop_fn pop; \
    top_fn  top; \
    empty_fn empty; \
    void *ctx; \
} Name; \

#define DECL_STACK_T(T) DECL_STACK_T_NAME(T, T##Stack)

#define STACK_PUSH(stack, value) ((stack)->push((stack)->ctx, (value)))
#define STACK_POP(stack) ((stack)->pop((stack)->ctx))
#define STACK_TOP(stack) ((stack)->top((stack)->ctx))
#define STACK_EMPTY(stack) ((stack)->empty((stack)->ctx))

_END_EXTERN_C
