#pragma once

#include <cpp-utils/preprocessor.h>

#define DLL_SHARED __attribute__((section ("shared"), shared))

// /////////////////////////////////////////////////////////////////////////////

#define Foreach(element,range) for (auto& element : range)
#define RangeArg(range) (range).begin(),(range).end()

// /////////////////////////////////////////////////////////////////////////////
// Check for 64bit

// Check GCC
#ifdef __GNUC__
  #if defined(__x86_64__) || defined(__ppc64__)
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


#if CPUBITSET == 32
# define MAIN_MODULE 1
#endif


#if CPUBITSET == 32
# define POWERWIN_APP_NAME "PowerWin32"
# define POWERWIN_64BIT_NAME "PowerWin64"
# define POWERWIN_64BIT_NAME_W CPP_TO_WIDESTRING(POWERWIN_64BIT_NAME)
#elif CPUBITSET == 64
# define POWERWIN_APP_NAME "PowerWin64"
#endif

#define POWERWIN_APP_NAME_W CPP_TO_WIDESTRING(POWERWIN_APP_NAME)

#ifdef BUILD_DLL
#define DLL_PUBLIC __declspec(dllexport)
#else
#define DLL_PUBLIC __declspec(dllimport)
#endif
