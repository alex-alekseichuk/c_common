#include "common/rb_tree.h"

typedef struct RbNode {
    struct RbNode *left;
    struct RbNode *right;
    struct RbNode *parent;
    unsigned char color;
} RbNode;

RbNode RB_NIL = {
    .left=NULL,
    .right=NULL,
    .parent=NULL,
    .color=RB_BLACK
}, *P_RB_NIL = &RB_NIL;

typedef struct RbTree {
    Allocator *alloc;
    cmp_fn cmp;
    RbNode *root;
} RbTree;

static void left_rotate(void *tree, void *node) {
    RbTree *t = (RbTree *)tree;
    RbNode *x = (RbNode *)node;
    RbNode *y = x->right;
    x->right = y->left;

    if (y->left != &RB_NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == &RB_NIL)
        t->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void right_rotate(void *tree, void *node) {
    RbTree *t = (RbTree *)tree;
    RbNode *y = (RbNode *)node;
    RbNode *x = y->left;
    y->left = x->right;

    if (x->right != &RB_NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == &RB_NIL)
        t->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}

void rb_insert_fixup_impl(void *tree, void *node) {
    RbTree *t = (RbTree *)tree;
    RbNode *z = (RbNode *)node;
    while (z->parent->color == RB_RED) {
        if (z->parent == z->parent->parent->left) {
            RbNode *y = z->parent->parent->right; // uncle

            if (y->color == RB_RED) { // case 1
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) { // case 2
                    z = z->parent;
                    left_rotate(t, z);
                }
                // case 3
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                right_rotate(t, z->parent->parent);
            }
        } else {
            RbNode *y = z->parent->parent->left;

            if (y->color == RB_RED) {
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(t, z);
                }
                z->parent->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = RB_BLACK;
}

static RbNode *tree_minimum(RbTree *t, RbNode *x) {
    (void)t;
    while (x->left != &RB_NIL)
        x = x->left;
    return x;
}

static void rb_transplant(RbTree *t, RbNode *u, RbNode *v) {
    if (u->parent == &RB_NIL)
        t->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    v->parent = u->parent;
}

static void rb_delete_fixup(RbTree *t, RbNode *x) {
    while (x != t->root && x->color == RB_BLACK) {
        if (x == x->parent->left) {
            RbNode *w = x->parent->right; // brother

            if (w->color == RB_RED) { // case 1
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                left_rotate(t, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == RB_BLACK && w->right->color == RB_BLACK) { // case 2
                w->color = RB_RED;
                x = x->parent;
            } else {
                if (w->right->color == RB_BLACK) { // case 3
                    w->left->color = RB_BLACK;
                    w->color = RB_RED;
                    right_rotate(t, w);
                    w = x->parent->right;
                }

                // case 4
                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->right->color = RB_BLACK;
                left_rotate(t, x->parent);
                x = t->root;
            }
        } else {
            RbNode *w = x->parent->left;

            if (w->color == RB_RED) {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                right_rotate(t, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == RB_BLACK && w->left->color == RB_BLACK) {
                w->color = RB_RED;
                x = x->parent;
            } else {
                if (w->left->color == RB_BLACK) {
                    w->right->color = RB_BLACK;
                    w->color = RB_RED;
                    left_rotate(t, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = RB_BLACK;
                w->left->color = RB_BLACK;
                right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RB_BLACK;
}

void rb_delete_impl(void *tree, void *node) {
    RbTree *t = (RbTree *)tree;
    RbNode *z = (RbNode *)node;
    RbNode *y = z;
    unsigned char y_original_color = y->color;
    RbNode *x;

    if (z->left == &RB_NIL) {
        x = z->right;
        rb_transplant(t, z, z->right);
    } else if (z->right == &RB_NIL) {
        x = z->left;
        rb_transplant(t, z, z->left);
    } else {
        y = tree_minimum(t, z->right); // successor
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else {
            rb_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rb_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == RB_BLACK)
        rb_delete_fixup(t, x);
}
