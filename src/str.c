#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "common/allocator.h"

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
    char *data;
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




/*
    char *line = NULL;
    size_t len = 0;

    ssize_t nread = read_line(&line, &len, stdin);

    if (nread != -1) {
        printf("Read: %s", line);
    }

    free(line);
*/
size_t read_line(char **lineptr, size_t *n, FILE *stream) {
    if (!lineptr || !n || !stream) return -1;

    size_t size = (*lineptr && *n > 0) ? *n : 128;

    if (*lineptr == NULL) {
        *lineptr = malloc(size);
        if (!*lineptr) return -1;
        *n = size;
    }

    size_t len = 0;
    int c;

    while ((c = fgetc(stream)) != EOF) {
        if (len + 1 >= *n) {
            size_t new_size = *n * 2;
            char *tmp = realloc(*lineptr, new_size);
            if (!tmp) return -1;

            *lineptr = tmp;
            *n = new_size;
        }

        (*lineptr)[len++] = (char)c;

        if (c == '\n')
            break;
    }

    if (len == 0 && c == EOF)
        return -1;

    (*lineptr)[len] = '\0';
    return (size_t)len;
}

int str_split(const char *str, char *tokens[], int size) {
    int count = 0;
    char *token = strtok(str, " \n");
    while (token != NULL && count < size) {
        tokens[count++] = token;
        token = strtok(NULL, " \n");
    }
    return count;
}

char **malloc_split(const char *str, int *out_count) {
    int capacity = 10;
    int count = 0;
    char **result = malloc(capacity * sizeof(char*));

    const char *start = str;

    while (*str) {
        // skip spaces
        while (*str == ' ') str++;

        if (*str == '\0') break;

        start = str;

        // find word end
        while (*str && *str != ' ') str++;

        int len = str - start;

        char *word = malloc(len + 1);
        memcpy(word, start, len);
        word[len] = '\0';

        if (count >= capacity) {
            capacity *= 2;
            result = realloc(result, capacity * sizeof(char*));
        }

        result[count++] = word;
    }

    *out_count = count;
    return result;
}
