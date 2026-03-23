#pragma once

#include <stddef.h>
#include <stdarg.h>
#include "common/common.h"

_BEGIN_EXTERN_C

COMMON_API size_t utf8_strlen(const char *s);
COMMON_API char* malloc_printf(const char *fmt, ...);

typedef struct Str Str;

_END_EXTERN_C
