#pragma once

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
