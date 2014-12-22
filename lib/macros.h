#pragma once

#define DLL_SHARED __attribute__((section ("shared"), shared))

// /////////////////////////////////////////////////////////////////////////////

#define Foreach(element,range) for (auto&& element : range)
#define RangeArg(range) (range).begin(),(range).end()

// /////////////////////////////////////////////////////////////////////////////
// Check for 64bit

// Check GCC
#if __defined__(__GNUC__)
  #if __defined__(__x86_64__) || __defined__(__ppc64__)
    #define CPUBITSET 64
  #else
    #define CPUBITSET 32
  #endif
#else

  // Check windows
  #ifdef _WIN32
    #ifdef _WIN64
      #define CPUBITSET 64
    #else
      #define CPUBITSET 32
  #endif
#endif

#endif

#ifndef CPUBITSET
    #error "Must define CPUBITSET"
#endif
