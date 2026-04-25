#include "common/list.h"

typedef struct ListNode {
    struct ListNode *next;
} ListNode;

typedef struct List {
    Allocator *alloc;
    ListNode *tail;
} List;

void list_free(void *list) {
    List *_list = (List *)list;
    ListNode *node = _list->tail;
    while (node) {
        ListNode* next_node = _list->tail == node->next ? NULL : node->next;
        FREE(_list->alloc, node);
        node = next_node;
    }
    _list->tail = NULL;
}

void list_insert_head(void *list, void *new_node) {
    List *_list = (List *)list;
    ListNode *_new_node = (ListNode *)new_node;
    if (_list->tail == NULL) {
        _list->tail = _new_node;
        _new_node->next = _new_node;
        return;
    }

    _new_node->next = _list->tail->next;
    _list->tail->next = _new_node;
}

void list_append(void *list, void *new_node, void *after_node) {
    List *_list = (List *)list;
    ListNode *_new_node = (ListNode *)new_node;
    ListNode *_after_node = (ListNode *)after_node;

    if (_list->tail == NULL) {
        _list->tail = _new_node;
        _new_node->next = _new_node;
        return;
    }

    if (_after_node == NULL) {
        _after_node = _list->tail;
    }

    if (_after_node == _list->tail) {
        _list->tail = _new_node;
    }

    _new_node->next = _after_node->next;
    _after_node->next = _new_node;
}

void list_free_head(void *list) {
    List *_list = (List *)list;
    
    if (_list->tail == NULL) return;

    ListNode *head = _list->tail->next;
    if (head == _list->tail) {
        _list->tail = NULL;
    } else {
        _list->tail->next = head->next;
    }

    FREE(_list->alloc, head);
}

void list_reverse(void *list) {
    List *_list = (List *)list;

    if (_list->tail == NULL || _list->tail == _list->tail->next) return;

    ListNode *prev = _list->tail;
    ListNode *current = prev->next;
    ListNode *next = current->next;
    while (prev != next) {
        current->next = prev;
        prev = current;
        current = next;
        next = next->next;
    }
    _list->tail = current;
}
