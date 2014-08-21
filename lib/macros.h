#pragma once

#define DLL_SHARED __attribute__((section ("shared"), shared))

// /////////////////////////////////////////////////////////////////////////////

#define Foreach(element,range) for (auto&& element : range)
#define RangeArg(range) (range).begin(),(range).end()

// /////////////////////////////////////////////////////////////////////////////
// Check for 64bit

// Check windows
#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

// Check GCC
#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

#if !defined(ENV64BIT) && !defined(ENV32BIT)
    #error "Must define either ENV32BIT or ENV64BIT"
#endif
