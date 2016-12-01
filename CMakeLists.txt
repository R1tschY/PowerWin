# detect non-Windows and use cross-mingw64
if (NOT WIN32 AND NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Toolchain-cross-mingw.cmake")
else()
  if (PowerWin_ArchBuild)
    set(CMAKE_RC_COMPILER ${COMPILER_PREFIX}-windres)
    set(CMAKE_C_COMPILER ${COMPILER_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER ${COMPILER_PREFIX}-g++)
  endif()
endif()

# cmake version
cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.2)
cmake_policy(SET CMP0048 NEW)

# project
project(PowerWin VERSION 0.3)
set(PowerWin_VERSION_EXT "alpha")

set(PowerWin_AUTHOR "Richard Liebscher")
set(PowerWin_COPYRIGHT "Copyright (C) 2016 Richard Liebscher <r1tschy@yahoo.de>")
set(PowerWin_URL "https://github.com/R1tschY/PowerWin")
set(PowerWin_LICENCE_URL "https://www.gnu.org/licenses/gpl-3.0.html")
set(PowerWin_LICENCE_PATH "GPL.txt")
set(PowerWin_DESCRIPTION_EN "Extents Windows with some cool features")
set(PowerWin_DESCRIPTION_DE "Erweitert Windows um ein paar coole Fähigkeiten")

set(PowerWin_VERSIONINFO_ARGS
  RC_FILE_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/versioninfo.rc.in"
  COMPANY_NAME "${PowerWin_AUTHOR}"
  LEGAL_COPYRIGHT "${PowerWin_COPYRIGHT}"
)
set(PowerWin_VERSION_NOEXT "${PowerWin_VERSION}")
if (PowerWin_VERSION_EXT)
  list(APPEND PowerWin_VERSIONINFO_ARGS PRERELEASE)
  set(PowerWin_VERSION "${PowerWin_VERSION_NOEXT}${PowerWin_VERSION_EXT}")
  set(PROJECT_VERSION "${PowerWin_VERSION_NOEXT}${PowerWin_VERSION_EXT}")
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  list(APPEND PowerWin_VERSIONINFO_ARGS DEBUG)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(IsolatedProject)

if (NOT PowerWin_ArchBuild)

  message(STATUS "    ____                         _       ___      ")     
  message(STATUS "   / __ \\____ _      _____  ____| |     / (_)___  ")
  message(STATUS "  / /_/ / __ \\ | /| / / _ \\/ ___/ | /| / / / __ \\ ")
  message(STATUS " / ____/ /_/ / |/ |/ /  __/ /   | |/ |/ / / / / / ")
  message(STATUS "/_/    \\____/|__/|__/\\___/_/    |__/|__/_/_/ /_/  ")
  message(STATUS "                                                  ")

  add_isolated_project(
    TARGET PowerWin_i686 ADD_TO_ALL
    DIR "${PROJECT_BINARY_DIR}/i686"
    
    PASSTHROUGH 
    "CMAKE_BUILD_TYPE"
    "CMAKE_TOOLCHAIN_FILE"
    "BOOST_ROOT"
    "POWERWIN_I18N"
    
    NEW
    "PowerWin_ArchBuild=i686" 
    "COMPILER_PREFIX=i686-w64-mingw32"
    
    COMMENT "Build i686 targets" 
  )

  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_isolated_project(
      TARGET PowerWin_x86_64 ADD_TO_ALL
      DIR "${PROJECT_BINARY_DIR}/x86_64"
    
      PASSTHROUGH 
      "CMAKE_BUILD_TYPE"
      "CMAKE_TOOLCHAIN_FILE"
      "BOOST_ROOT"
      "POWERWIN_I18N"
    
      NEW
      "PowerWin_ArchBuild=x86_64" 
      "COMPILER_PREFIX=x86_64-w64-mingw32"
    
      COMMENT "Build x86_64 targets"                              
    )
  endif()
  
  # cpack support
  install(
    FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/${PowerWin_LICENCE_PATH}"
    "${PROJECT_BINARY_DIR}/i686/bin/PowerWinApp.exe"
    "${PROJECT_BINARY_DIR}/i686/bin/libpowerwin32.dll"
    "${PROJECT_BINARY_DIR}/x86_64/bin/libpowerwin64.dll"
    
    DESTINATION ".")
     
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
  include(CPack)
    
  return()
endif()

# include util modules
include(Listx)
include(AddCXXFlags)
include(AddConfig)
include(LTO)
include(CompileForWindows)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(STATUS "   ________   __    _ __     ")
  message(STATUS "  |__  /__ \\ / /_  (_) /_   ")
  message(STATUS "   /_ <__/ // __ \\/ / __/   ")
  message(STATUS " ___/ / __// /_/ / / /_      ")
  message(STATUS "/____/____/_.___/_/\\__/     ")
  message(STATUS "                             ")
else()
  message(STATUS "   _____ __ __  __    _ __   ")
  message(STATUS "  / ___// // / / /_  (_) /_  ") 
  message(STATUS " / __ \\/ // /_/ __ \\/ / __/")
  message(STATUS "/ /_/ /__  __/ /_/ / / /_    ")
  message(STATUS "\\____/  /_/ /_.___/_/\\__/  ")
  message(STATUS "                             ")
endif()                       

color_message(red "Build for ${PowerWin_ArchBuild}")
color_message(red "C++ Compiler: ${CMAKE_CXX_COMPILER}")
color_message(red "RC Compiler: ${CMAKE_RC_COMPILER}")
color_message(red "=====")

# Boost
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON) 
if (POWERWIN_I18N)
  find_package(Boost REQUIRED COMPONENTS locale)
else()
  find_package(Boost REQUIRED)    
endif()
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# C++14
#set_property(GLOBAL PROPERTY CXX_STANDARD 14)
#set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)
add_flags(CXX "-std=gnu++14")

# stdlibs
add_flags(LD "-static-libgcc -static-libstdc++")

# Windows
compile_for_windows(UNICODE STRICT VERSION Vista)

# Debug mode
add_config(
  Debug 
  CXX_FLAGS "-Og -g -DDEBUG -D_DEBUG" 
  LINKER_FLAGS "-Og -g"
)

# Release mode
add_config(
  Release 
  CXX_FLAGS "-O2 -DNDEBUG" 
  LINKER_FLAGS "-s -O2"
)
enable_lto(CONFIG Release)

# Warnings
add_cxx_flags("-Wall -Wextra -Wshadow -Wcast-align -Wunused -Wcast-qual -Wconversion -Wno-unused-parameter")
add_cxx_flag_checked("-Wnon-virtual-dtor")
add_cxx_flag_checked("-Woverloaded-virtual")
add_cxx_flag_checked("-Wold-style-cast")     
add_cxx_flag_checked("-Wsuggest-override")
add_cxx_flag_checked("-Wundef")
add_cxx_flag_checked("-Wconditionally-supported")
add_cxx_flag_checked("-Wzero-as-null-pointer-constant")
add_cxx_flag_checked("-Wnull-dereference") # gcc 6
add_cxx_flag_checked("-Wduplicated-cond") # gcc 6
add_cxx_flag_checked("-Wshift-negative-value") # gcc 6

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# libs
add_subdirectory(libs/c++)
add_subdirectory(libs/lightports)

add_definitions(-DCPP_ASSERT_FAIL=CPP_ASSERT_FAIL_THROW)

# config
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/powerwin-config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/powerwin-config.h)


include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# modules
add_subdirectory(modules)

# hooklib
add_subdirectory(hooklib)

# app
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
  add_subdirectory(app)
endif()
