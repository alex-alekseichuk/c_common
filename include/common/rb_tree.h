#pragma once

#include <stddef.h>
#include "common/allocator.h"
#include "common/common.h"

_BEGIN_EXTERN_C

typedef int(*cmp_fn)(void *a, void *b);

#define RB_RED 1
#define RB_BLACK 0

typedef struct RbNode {
    struct RbNode *left;
    struct RbNode *right;
    struct RbNode *parent;
    unsigned char color;
} RbNode;

extern RbNode RB_NIL;

typedef struct RbTree {
    Allocator *alloc;
    cmp_fn cmp;
    RbNode *root;
} RbTree;

COMMON_API void rb_insert_fixup(void *tree, void *node);
COMMON_API void rb_delete(void *tree, void *node);

#define DEFINE_RB_TREE(T)                                                   \
typedef struct T##RbNode {                                                  \
    struct T##RbNode *left;                                                 \
    struct T##RbNode *right;                                                \
    struct T##RbNode *parent;                                               \
    unsigned char color;                                                    \
    T value;                                                                \
} T##RbNode;                                                                \
                                                                            \
typedef struct T##RbTree {                                                  \
    Allocator *alloc;                                                       \
    cmp_fn cmp;                                                             \
    T##RbNode *root;                                                        \
} T##RbTree;                                                                \
                                                                            \
COMMON_API T##RbTree make_##T##RbTree(Allocator *alloc, cmp_fn cmp);        \
COMMON_API int T##RbTree##_insert(T##RbTree *tree, T value);                \
COMMON_API T##RbNode *T##RbTree##_search(T##RbTree *tree, T value);         \
COMMON_API T T##RbTree##_delete(T##RbTree *tree, T##RbNode *node);          \

#define IMPL_RB_TREE(T)                                                     \
static T##RbNode *T##RbNIL = (T##RbNode *)&RB_NIL;                          \
T##RbTree make_##T##RbTree(Allocator *alloc, cmp_fn cmp) {                  \
    return (T##RbTree){.alloc=alloc, .cmp=cmp, .root=T##RbNIL};             \
}                                                                           \
                                                                            \
int T##RbTree##_insert(T##RbTree *tree, T value) {                          \
    T##RbNode *z = ALLOC_T(tree->alloc, T##RbNode);                         \
    if (!z) return 0;                                                       \
    z->left = T##RbNIL;                                                     \
    z->right = T##RbNIL;                                                    \
    z->parent = T##RbNIL;                                                   \
    z->color = RB_RED;                                                      \
    z->value = value;                                                       \
    T##RbNode *y = T##RbNIL;                                                \
    T##RbNode *x = tree->root;                                              \
                                                                            \
    while (x != T##RbNIL) {                                                 \
        y = x;                                                              \
        if (tree->cmp(&z->value, &x->value) < 0)                            \
            x = x->left;                                                    \
        else                                                                \
            x = x->right;                                                   \
    }                                                                       \
                                                                            \
    z->parent = y;                                                          \
                                                                            \
    if (y == T##RbNIL)                                                      \
        tree->root = z;                                                     \
    else if (tree->cmp(&z->value, &y->value) < 0)                           \
        y->left = z;                                                        \
    else                                                                    \
        y->right = z;                                                       \
                                                                            \
    rb_insert_fixup(tree, z);                                               \
    return 1;                                                               \
}                                                                           \
                                                                            \
T##RbNode *T##RbTree##_search(T##RbTree *tree, T value) {                   \
    T##RbNode *cur = tree->root;                                            \
    while (cur != T##RbNIL && tree->cmp(&value, &cur->value) != 0) {        \
        if (tree->cmp(&value, &cur->value) < 0)                             \
            cur = cur->left;                                                \
        else                                                                \
            cur = cur->right;                                               \
    }                                                                       \
    return cur;                                                             \
}                                                                           \
                                                                            \
T T##RbTree##_delete(T##RbTree *tree, T##RbNode *node) {                    \
    if (tree->root == T##RbNIL || node == T##RbNIL || node == NULL) return (T){0}; \
    T value = node->value;                                                  \
    rb_delete(tree, node);                                                  \
    FREE(tree->alloc, node);                                                \
    return value;                                                           \
}                                                                           \

_END_EXTERN_C
