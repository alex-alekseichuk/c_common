#pragma once
/**
 * Circular linked list interface and implementation for a list of type T with allocator.
 */

#include <stddef.h>
#include "common/common.h"
#include "common/allocator.h"

_BEGIN_EXTERN_C

typedef struct ListNode ListNode;
typedef struct List List;

COMMON_API void list_free(void *list);
COMMON_API void list_insert_head(void *list, void *new_node);
COMMON_API void list_append(void *list, void *new_node, void *after_node);
COMMON_API void list_free_head(void *list);
COMMON_API void list_reverse(void *list);

#define DECL_LIST_T(T)                                                       \
typedef struct T##ListNode T##ListNode;                                      \
typedef struct T##List T##List;                                              \
                                                                             \
COMMON_API T##List make_##T##List(Allocator *alloc);                         \
COMMON_API int T##List##_insert_head(T##List *list, T value);                \
COMMON_API int T##List##_append(T##List *list, T value, T##ListNode *after_node); \
COMMON_API T T##List##_remove_head(T##List *list);                           \
COMMON_API T T##List##_head(T##List *list);                                  \
COMMON_API T T##List##_tail(T##List *list);                                  \

#define IMPL_LIST_T(T)                                                       \
typedef struct T##ListNode {                                                 \
    struct T##ListNode *next;                                                \
    T value;                                                                 \
} T##ListNode;                                                               \
                                                                             \
typedef struct T##List {                                                     \
    Allocator *alloc;                                                        \
    T##ListNode *tail;                                                       \
} T##List;                                                                   \
                                                                             \
T##List make_##T##List(Allocator *alloc) {                                   \
    return (T##List){.alloc=alloc,.tail=NULL};                               \
}                                                                            \
                                                                             \
int T##List##_insert_head(T##List *list, T value) {                          \
    T##ListNode *new_node = ALLOC_T(list->alloc, T##ListNode);               \
    if (!new_node) return 0;                                                 \
    new_node->value = value;                                                 \
    list_insert_head(list, new_node);                                        \
    return 1;                                                                \
}                                                                            \
                                                                             \
int T##List##_append(T##List *list, T value, T##ListNode *after_node) {      \
    T##ListNode *new_node = ALLOC_T(list->alloc, T##ListNode);               \
    if (!new_node) return 0;                                                 \
    new_node->value = value;                                                 \
    list_append(list, new_node, after_node);                                 \
    return 1;                                                                \
}                                                                            \
                                                                             \
T T##List##_remove_head(T##List *list) {                                     \
    if (list->tail == NULL) return (T){0};                                   \
    T value = list->tail->next->value;                                       \
    list_free_head(list);                                                    \
    return value;                                                            \
}                                                                            \
                                                                             \
T T##List##_head(T##List *list) {                                            \
    if (list->tail == NULL) return (T){0};                                   \
    return list->tail->next->value;                                          \
}                                                                            \
                                                                             \
T T##List##_tail(T##List *list) {                                            \
    if (list->tail == NULL) return (T){0};                                   \
    return list->tail->value;                                                \
}                                                                            \

_END_EXTERN_C
