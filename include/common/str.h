#pragma once
/**
 * Dynamic String
 *
int main() {
    char *s = make_str();

    s = str_cat(s, "Hello");
    s = str_cat(s, ", ");
    s = str_cat(s, "world!");

    printf("%s\n", s);
    printf("len=%zu cap=%zu\n", str_len(s), str_size(s));

    str_free(s);
}
 * 
 */

#include <stddef.h>
#include "common/common.h"
#include "common/allocator.h"

_BEGIN_EXTERN_C

COMMON_API char *make_str(void);
COMMON_API char *make_str_alloc(Allocator *a);
COMMON_API void  str_free(char *s);

COMMON_API char *str_resize(char *s, size_t additional);

COMMON_API char *str_cat(char *s, const char *cstr);
COMMON_API char *str_cat_n(char *s, const char *data, size_t n);
COMMON_API char *str_cpy(char *s, const char *cstr);

COMMON_API size_t str_len(const char *s);
COMMON_API size_t str_size(const char *s);

_END_EXTERN_C
