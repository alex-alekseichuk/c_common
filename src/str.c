#include <stdlib.h>
#include <string.h>
#include "common/allocator.h"
#include "common/str.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

typedef struct {
    size_t len;
    size_t size;
    const Allocator *alloc;
    char data[];
} str_hdr;

static str_hdr *str__hdr(char *s) {
    return s ? container_of(s, str_hdr, data) : NULL;
}

static const str_hdr *str__hdr_const(const char *s) {
    return s ? container_of(s, str_hdr, data) : NULL;
}

static size_t str__grow_size(size_t size, size_t min_needed) {
    size_t new_size = size ? size : 16;
    while (new_size < min_needed) {
        if (new_size < 1024) new_size *= 2;
        else new_size = new_size * 125 / 100;
    }
    return new_size;
}

static char *str__resize(char *s, size_t new_size) {
    str_hdr *hdr = str__hdr(s);
    size_t size = sizeof(str_hdr) + new_size + 1;

    str_hdr *new_hdr = REALLOC(hdr->alloc, hdr, size);
    if (!new_hdr) return NULL;

    new_hdr->size = new_size;
    return new_hdr->data;
}

char *make_str_alloc(const Allocator *a) {
    if (!a) a = &sys_alloc;

    size_t size = 16;
    size_t _size = sizeof(str_hdr) + size + 1;

    str_hdr *hdr = ALLOC(a, _size);
    if (!hdr) return NULL;

    hdr->len = 0;
    hdr->size = size;
    hdr->alloc = a;
    hdr->data[0] = '\0';

    return hdr->data;
}

char *make_str(void) {
    return make_str_alloc(&sys_alloc);
}

void str_free(char *s) {
    if (!s) return;
    str_hdr *hdr = str__hdr(s);
    FREE(hdr->alloc, hdr);
}

char *str_resize(char *s, size_t new_size) {
    if (!s) return NULL;

    str_hdr *hdr = str__hdr(s);

    if (new_size <= hdr->size) return s;

    new_size = str__grow_size(hdr->size, new_size);
    return str__resize(s, new_size);
}

char *str_cat_n(char *s, const char *data, size_t n) {
    if (!s || !data) return s;

    str_hdr *hdr = str__hdr(s);

    s = str_resize(s, hdr->len + n);
    if (!s) return NULL;

    memcpy(s + hdr->len, data, n);
    hdr->len += n;
    s[hdr->len] = '\0';

    return s;
}

char *str_cat(char *s, const char *cstr) {
    if (!cstr) return s;
    return str_cat_n(s, cstr, strlen(cstr));
}

char *str_cpy(char *s, const char *cstr) {
    if (!s || !cstr) return s;

    size_t n = strlen(cstr);
    s = str_resize(s, n);
    if (!s) return NULL;

    str_hdr *hdr = str__hdr(s);
    memcpy(s, cstr, n);
    hdr->len = n;
    s[n] = '\0';

    return s;
}

size_t str_len(const char *s) {
    const str_hdr *hdr = str__hdr_const(s);
    return hdr ? hdr->len : 0;
}

size_t str_size(const char *s) {
    const str_hdr *hdr = str__hdr_const(s);
    return hdr ? hdr->size : 0;
}

#pragma GCC diagnostic pop

