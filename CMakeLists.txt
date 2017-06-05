# cmake version
cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.2)
cmake_policy(SET CMP0048 NEW)

if (NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" PATH "Installation directory")
endif()

if (NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" "Build type (Debug or Release)")
endif()

# project
project(PowerWin VERSION 0.4)
set(PowerWin_VERSION_EXT "alpha")

set(PowerWin_AUTHOR "Richard Liebscher")
set(PowerWin_COPYRIGHT "Copyright (C) 2017 Richard Liebscher <r1tschy@yahoo.de>")
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

if (NOT PowerWin_ArchBuild)

  message(STATUS "    ____                         _       ___      ")     
  message(STATUS "   / __ \\____ _      _____  ____| |     / (_)___  ")
  message(STATUS "  / /_/ / __ \\ | /| / / _ \\/ ___/ | /| / / / __ \\ ")
  message(STATUS " / ____/ /_/ / |/ |/ /  __/ /   | |/ |/ / / / / / ")
  message(STATUS "/_/    \\____/|__/|__/\\___/_/    |__/|__/_/_/ /_/  ")
  message(STATUS "                                                  ")
  
  # Options
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DEFAULT_STATIC_QT ON)
  else()
    set(DEFAULT_STATIC_QT OFF)
  endif()
      
  option(STATIC_QT "Use static Qt5 libraries" "${DEFAULT_STATIC_QT}")
  
  # detect host system
  if (UNIX)
    # MSYS2
    if (MSYS)
      message(STATUS "Detected MSYS2")
      set(PowerWin_COMPILER_PREFIX_i686 "i686-w64-mingw32")
      set(PowerWin_COMPILER_PREFIX_x86_64 "x86_64-w64-mingw32")
      set(PowerWin_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain-msys2-mingw.cmake")
      set(PowerWin_CMAKE_i686 "${CMAKE_SOURCE_DIR}/cmake/msys2-mingw32-cmake.sh")
      set(PowerWin_CMAKE_x86_64 "${CMAKE_SOURCE_DIR}/cmake/msys2-mingw64-cmake.sh")
        
    # Linux
    elseif (NOT APPLE)
      message(STATUS "Detected Linux")
      set(PowerWin_COMPILER_PREFIX_i686 "i686-w64-mingw32")
      set(PowerWin_COMPILER_PREFIX_x86_64 "x86_64-w64-mingw32")
      set(PowerWin_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain-linux-mingw.cmake")
      set(PowerWin_CMAKE_i686 "${CMAKE_COMMAND}")
      set(PowerWin_CMAKE_x86_64 "${CMAKE_COMMAND}")
    endif()
  else()
    message(FATAL_ERROR "host system not supported. Use MSYS2 or MinGW on Linux.")
    return()
  endif()
  
  include(IsolatedProject)

  add_isolated_project(
    CMAKE "${PowerWin_CMAKE_i686}"
    TARGET PowerWin_i686 ADD_TO_ALL
    DIR "${PROJECT_BINARY_DIR}/i686"
    
    PASSTHROUGH 
    "CMAKE_BUILD_TYPE"
    "BOOST_ROOT"
    "STATIC_QT"
    
    NEW
    "CPP_UTILS_NO_TESTS=ON"
    "PowerWin_ArchBuild=i686" 
    "COMPILER_PREFIX=${PowerWin_COMPILER_PREFIX_i686}"
    "CMAKE_TOOLCHAIN_FILE=${PowerWin_TOOLCHAIN_FILE}"
    
    COMMENT "Build i686 targets" 
  )

  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_isolated_project(
      CMAKE "${PowerWin_CMAKE_x86_64}"
      TARGET PowerWin_x86_64 ADD_TO_ALL
      DIR "${PROJECT_BINARY_DIR}/x86_64"
    
      PASSTHROUGH 
      "CMAKE_BUILD_TYPE"
      "BOOST_ROOT"
    
      NEW
      "CPP_UTILS_NO_TESTS=ON"
      "PowerWin_ArchBuild=x86_64" 
      "COMPILER_PREFIX=${PowerWin_COMPILER_PREFIX_x86_64}"
      "CMAKE_TOOLCHAIN_FILE=${PowerWin_TOOLCHAIN_FILE}"
    
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
    DESTINATION .
  )
     
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
  set(CPACK_PACKAGE_VENDOR "${PowerWin_AUTHOR}")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION_EN}")
  set(CPACK_PACKAGE_EXECUTABLES "PowerWinApp;PowerWin")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
  set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${PROJECT_NAME}")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/GPL.txt")
  set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
  
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
  
  set(CPACK_MONOLITHIC_INSTALL ON)
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
find_package(Boost REQUIRED)    
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# Qt
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    if (STATIC_QT)     
      set(PowerWin_LINKER_FLAGS -static -static-libstdc++ -static-libgcc)
    endif()
    
    find_package(Qt5Widgets REQUIRED)

    add_definitions(
     -DQT_DEPRECATED_WARNINGS
     -DQT_DISABLE_DEPRECATED_BEFORE=0x050500
     -DQT_USE_QSTRINGBUILDER)
endif()

# C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows
compile_for_windows(UNICODE STRICT VERSION 7)

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
