#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(_WIN32) && defined(COMMON_BUILD_DLL)
    #define COMMON_API __declspec(dllexport)
#elif defined(_WIN32)
    #define COMMON_API __declspec(dllimport)
#else
    #define COMMON_API
#endif

COMMON_API int func(int n);

#endif
