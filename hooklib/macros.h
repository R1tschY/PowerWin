#pragma once

#include <cpp-utils/preprocessor.h>

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
# define POWERWIN_APP_NAME "PowerWin32"
# define POWERWIN_64BIT_NAME "PowerWin64"
#elif CPUBITSET == 64
# define POWERWIN_APP_NAME "PowerWin64"
#endif

#ifdef BUILD_DLL
#define DLL_PUBLIC __declspec(dllexport)
#else
#define DLL_PUBLIC __declspec(dllimport)
#endif
