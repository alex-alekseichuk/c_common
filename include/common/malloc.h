#pragma once

#include "common/allocator.h"

extern Allocator make_malloc_allocator();

/*
#include "allocator.h"
#include "malloc.h"

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node* list_push(Allocator *a, Node *head, int value) {
    Node *n = a->alloc(a->ctx, sizeof(Node));
    if (!n) return head;

    n->value = value;
    n->next  = head;
    return n;
}

Allocator a = make_malloc_allocator();

Node *list = NULL;
list = list_push(&a, list, 1);
list = list_push(&a, list, 2);
list = list_push(&a, list, 3);
*/
