#pragma once
#include <time.h>
#include "common/common.h"

_BEGIN_EXTERN_C

/*
 * epoch_time.h — portable (Linux/OSX/Windows) Unix epoch time utilities
 * Supports:
 *   - Standard C (time.h)
 *   - POSIX (clock_gettime)
 *   - Windows (GetSystemTimeAsFileTime)
 *
 * API:
 *   long long epoch_seconds(void);
 *   long long epoch_millis(void);
 *   long long epoch_nanos(void);
 *
 * All return time since Unix epoch (1970-01-01 UTC)
 */

/* ========================= Seconds ========================= */
static inline long long epoch_seconds(void) {
    return (long long)time(NULL);
}

/* ========================= Platform detection ========================= */
#if defined(_WIN32) || defined(_WIN64)
    #define EPOCH_WINDOWS
#elif defined(__unix__) || defined(__APPLE__)
    #define EPOCH_POSIX
#else
    #define EPOCH_FALLBACK
#endif

/* ========================= Milliseconds ========================= */

#if defined(EPOCH_WINDOWS)

#include <windows.h>

static inline long long epoch_millis(void) {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    unsigned long long t = ((unsigned long long)ft.dwHighDateTime << 32) | ft.dwLowDateTime;

    /* Windows epoch is Jan 1, 1601 */
    const unsigned long long EPOCH_DIFF = 116444736000000000ULL;

    t -= EPOCH_DIFF;      /* to Unix epoch */
    t /= 10000ULL;       /* to milliseconds */

    return (long long)t;
}

static inline long long epoch_nanos(void) {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    unsigned long long t = ((unsigned long long)ft.dwHighDateTime << 32) | ft.dwLowDateTime;

    const unsigned long long EPOCH_DIFF = 116444736000000000ULL;

    t -= EPOCH_DIFF;
    t *= 100ULL; /* 100-ns units -> ns */

    return (long long)t;
}

#elif defined(EPOCH_POSIX)

#include <sys/time.h>

static inline long long epoch_millis(void) {
    #if defined(CLOCK_REALTIME)
        struct timespec ts;
        if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
            return (long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
        }
    #endif

    /* fallback to gettimeofday */
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;
}

static inline long long epoch_nanos(void) {
    #if defined(CLOCK_REALTIME)
        struct timespec ts;
        if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
            return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
        }
    #endif

    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000000LL + (long long)tv.tv_usec * 1000LL;
}

#else /* fallback: only seconds precision */

static inline long long epoch_millis(void) {
    return epoch_seconds() * 1000LL;
}

static inline long long epoch_nanos(void) {
    return epoch_seconds() * 1000000000LL;
}

#endif

#define MICROSEC_AS_NSEC 1000
#define SEC_AS_MICROSEC 1000000
#define SEC_AS_NSEC 1000000000

// returns something like 5.2017 seconds
static double timesec_diff(struct timesec start, struct timesec end) {
	struct timesec temp;
	if (end.tv_nsec - start.tv_nsec < 0) {
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = SEC_AS_NSEC + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	double msec = temp.tv_sec * SEC_AS_MICROSEC + temp.tv_nsec / MICROSEC_AS_NSEC;
	return msec / SEC_AS_MICROSEC;
}

_END_EXTERN_C

