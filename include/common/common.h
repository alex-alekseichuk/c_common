#pragma once
#include <stdalign.h>

#ifdef __cplusplus
    #define _BEGIN_EXTERN_C extern "C" {
    #define _END_EXTERN_C }
#else
    #define _BEGIN_EXTERN_C
    #define _END_EXTERN_C
#endif

#if defined(_WIN32) && defined(COMMON_SHARED)
  #if defined(COMMON_BUILD_DLL)
    #define COMMON_API __declspec(dllexport)
  #else
    #define COMMON_API __declspec(dllimport)
  #endif
#else
  #define COMMON_API
#endif

/*
Example of a function declaration in the headers:
COMMON_API int func(int n);
*/

extern const size_t SIZE_T; // 8
extern const size_t ALIGN_SIZE; // 32
#define ALIGN(size) (((size) + ALIGN_SIZE - 1) & ~(ALIGN_SIZE - 1))

#define BUFFER(NAME, T, SIZE) alignas(max_align_t) T NAME[(SIZE)]

#define BASE(self) (&((self)->base))

#if defined(__GNUC__) || defined(__clang__)
#define container_of(ptr, type, member) ({                  \
  const typeof(((type *)0)->member) *__mptr = (ptr);        \
  (type *)((char *)__mptr - offsetof(type, member));        \
})
#else
#define container_of(ptr, type, member) \
  ((type *)((char *)(ptr) - offsetof(type, member)))
#endif

#if defined(__GNUC__) || defined(__clang__)
#define container_of_arr(ptr, type, member) ({                  \
  const typeof(((type *)0)->member[0]) *__mptr = (ptr);        \
  (type *)((char *)__mptr - offsetof(type, member));        \
})
#else
#define container_of_arr(ptr, type, member) \
  ((type *)((char *)(ptr) - offsetof(type, member)))
#endif
