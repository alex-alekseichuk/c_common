#pragma once

#ifdef __cplusplus
    #define _BEGIN_EXTERN_C extern "C" {
    #define _END_EXTERN_C }
#else
    #define _BEGIN_EXTERN_C
    #define _END_EXTERN_C
#endif

#if defined(_WIN32) && defined(COMMON_BUILD_DLL)
    #define COMMON_API __declspec(dllexport) extern
#elif defined(_WIN32)
    #define COMMON_API __declspec(dllimport) extern
#else
    #define COMMON_API extern
#endif

/*
Example of a function declaration:
COMMON_API int func(int n);
*/

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
