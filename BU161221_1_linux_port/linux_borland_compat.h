// Small compatibility shim for Borland/Turbo C++ keywords when building with GCC/Clang.
#pragma once
#include <cstdlib>
#include <stdlib.h>   // for ::random(), ::srandom()

#ifndef farfree
#define farfree(p) free(p)
#endif

#ifndef farmalloc
#define farmalloc(n) malloc(n)
#endif

// Memory model keywords (DOS/16-bit) -> no-op on modern platforms
#ifndef far
#  define far
#endif
#ifndef near
#  define near
#endif
#ifndef huge
#  define huge
#endif

// Borland ISR calling convention keyword -> no-op for Linux builds
#ifndef interrupt
#  define interrupt
#endif

// Some Borland code expects 'cdecl' keyword
#ifndef cdecl
#  define cdecl
#endif


// Old code used nTested[]; this port uses hcTested[]

#include <cstdlib>
#include <ctime>
#include <cstring>

// ---- Borland-style structs (only if you use them) ----
// If you already defined these elsewhere, don't duplicate.
#ifndef BORLAND_TIME_STRUCT_DEFINED
#define BORLAND_TIME_STRUCT_DEFINED
struct time {
    unsigned char ti_hour;
    unsigned char ti_min;
    unsigned char ti_sec;
    unsigned char ti_hund; // 1/100 sec
};
#endif

// ---- gettime (Borland) -> uses std::time/localtime ----
static inline void gettime(struct time *t)
{
    std::time_t now = std::time(nullptr);
    std::tm lt{};
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS)
    localtime_r(&now, &lt);
#else
    std::tm *ptm = std::localtime(&now);
    if (ptm) lt = *ptm;
#endif
    t->ti_hour = static_cast<unsigned char>(lt.tm_hour);
    t->ti_min  = static_cast<unsigned char>(lt.tm_min);
    t->ti_sec  = static_cast<unsigned char>(lt.tm_sec);
    t->ti_hund = 0;
}

// ---- random(n) (Borland) -> rand()%n ----
// IMPORTANT: your code expects random(6) style.
// Linux has random() with NO args, so we provide the Borland one.
static inline long borland_random(long n)
{
   if (n <= 0) return 0;
    return std::rand() % n;
    // your existing implementation body, unchanged
}


#if 0
static inline long random(long n)
{
    if (n <= 0) return 0;
    return std::rand() % n;
}
#endif
// ---- _atold (Borland) -> strtod ----
static inline long double _atold(const char *s)
{
    if (!s) return 0.0L;
    return static_cast<long double>(std::strtod(s, nullptr));
}
static inline long b_random(long n)
{
    return (n <= 0) ? 0 : (::random() % n);
}
#ifdef random
#undef random
#endif
