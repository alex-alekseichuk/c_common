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
