#include <stddef.h>
#include <stdarg.h>
#include "allocator.h"

size_t utf8_strlen(const char *s) {
    size_t count = 0;
    while (*s) {
        if ((*s & 0xC0) != 0x80)
            count++;
        s++;
    }
    return count;
}

char* malloc_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    char *buf = malloc(n + 1);

    va_start(ap, fmt);
    vsnprintf(buf, n + 1, fmt, ap);
    va_end(ap);

    return buf;
}

typedef struct Str {
    size_t size;
    size_t len;
    char data[];
} Str;

Str *str(Allocator *a, const char *pchar) {
    size_t len = pchar ? strlen(pchar) : 0;
    Str *s = a->alloc(a->ctx, sizeof(Str) + len + 1);
    s->size = len;
    s->len = len;
    if (pchar)
        memcpy(s->data, pchar, len);
    s->data[len] = '\0';
    return s;
}

Str *str_n(Allocator *a, size_t size) {
    Str *s = a->alloc(a->ctx, sizeof(Str) + size + 1);
    s->size = size;
    s->len = 0;
    s->data[0] = '\0';
    return s;
}

str_cpy() {
    
}
