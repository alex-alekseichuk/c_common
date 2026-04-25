#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "common/allocator.h"
#include "common/array.h"
#include "common/array_impl.h"

size_t array_size(Array *arr) {return arr->size;}

size_t array_len(Array *arr) {return arr->len;}

int array_empty(Array *arr) {return arr->len == 0;}

void array_free(Array *arr) {
    if (!arr->data) return;
    FREE(arr->alloc, arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->len = 0;
}
