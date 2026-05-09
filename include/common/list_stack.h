#pragma once
/**
 * Stack interface implementation based on List data structure.
 */

#include <stddef.h>
#include "common/common.h"
#include "common/list.h"
#include "common/stack.h"

_BEGIN_EXTERN_C

#define DECL_LIST_STACK_T_NAME(T, StackName, ListName)              \
typedef struct List##StackName { \
    StackName base; \
    ListName *list; \
} List##StackName; \
COMMON_API List##StackName make_##StackName(ListName *list);              \

#define DECL_LIST_STACK_T(T) DECL_LIST_STACK_T_NAME(T, T##Stack, T##List)

#define IMPL_LIST_STACK_T_NAME(T, StackName, ListName)              \
static int StackName##_push(StackName *s, T value) {                   \
    return ListName##_insert_head(container_of(s, List##StackName, base)->list, value);          \
}                                                                   \
                                                                    \
static T StackName##_pop(StackName *s) {                               \
    return ListName##_remove_head(container_of(s, List##StackName, base)->list);                 \
}                                                                   \
                                                                    \
static T StackName##_top(StackName *s) {                               \
    return ListName##_head(container_of(s, List##StackName, base)->list);                        \
}                                                                   \
                                                                    \
static int StackName##_empty(StackName *s) {                           \
    return (container_of(s, List##StackName, base)->list)->tail == NULL;                         \
}                                                                   \
                                                                    \
static StackName##VTable vTable_##StackName = {                     \
    .push = StackName##_push,                               \
    .pop = StackName##_pop,                                 \
    .top = StackName##_top,                                 \
    .empty = StackName##_empty                              \
}; \
List##StackName make_##StackName(ListName *list) {                        \
    return (List##StackName){                                             \
        .base={.vTable = &vTable_##StackName},                      \
        .list = list                                                \
    };                                                              \
}                                                                   \

#define IMPL_LIST_STACK_T(T) IMPL_LIST_STACK_T_NAME(T, T##Stack, T##List)

_END_EXTERN_C
