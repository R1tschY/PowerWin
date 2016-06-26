# detect non-Windows and use cross-mingw64
if (NOT WIN32 AND NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Toolchain-cross-mingw.cmake")
endif()

# cmake version
cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.2)
cmake_policy(SET CMP0048 NEW)

# project
project(PowerWin VERSION 0.2)

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
add_library(boost INTERFACE)
target_include_directories(boost INTERFACE ${Boost_INCLUDE_DIRS})

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

# modules
add_subdirectory(modules)

# hooklib
add_subdirectory(hooklib)

# app
add_subdirectory(app)

# utils
add_custom_target(update COMMAND cmake .)
