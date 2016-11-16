# detect non-Windows and use cross-mingw64
if (NOT WIN32 AND NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Toolchain-cross-mingw.cmake")
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

# include util modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include(Listx)
include(AddCXXFlags)
include(AddConfig)
include(LTO)
include(CompileForWindows)

if (NOT(CMAKE_SIZEOF_VOID_P EQUAL 8))
  message(WARNING "You need a 64bit enviroment to compile the 64bit hook library.")
endif()

# Boost
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON) 
find_package(Boost REQUIRED)
#add_library(boost INTERFACE)
#target_include_directories(boost INTERFACE ${Boost_INCLUDE_DIRS})
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
  CXX_FLAGS "-Og -DDEBUG" 
  LINKER_FLAGS "-Og"
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
#add_cxx_flag_checked("-Wsuggest-final-methods")
#add_cxx_flag_checked("-Wsuggest-final-types")

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
add_subdirectory(app)

# utils
add_custom_target(update COMMAND cmake .)
