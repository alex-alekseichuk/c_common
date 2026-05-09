#pragma once
/**
 * Stack interface for a stack of type T.
 */

#include "common/common.h"

_BEGIN_EXTERN_C

#define DECL_STACK_T_NAME(T, StackName) \
typedef struct StackName StackName; \
typedef int (*push_fn)(StackName *s, T value); \
typedef T (*pop_fn)(StackName *s); \
typedef T (*top_fn)(StackName *s); \
typedef int (*empty_fn)(StackName *s); \
 \
typedef struct StackName##VTable { \
    push_fn push; \
    pop_fn pop; \
    top_fn  top; \
    empty_fn empty; \
} StackName##VTable; \
typedef struct StackName { \
    StackName##VTable *vTable; \
} StackName; \

#define DECL_STACK_T(T) DECL_STACK_T_NAME(T, T##Stack)

#define STACK_PUSH(stack, value) ((stack)->vTable->push((stack), (value)))
#define STACK_POP(stack) ((stack)->vTable->pop((stack)))
#define STACK_TOP(stack) ((stack)->vTable->top((stack)))
#define STACK_EMPTY(stack) ((stack)->vTable->empty((stack)))

_END_EXTERN_C
