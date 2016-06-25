# explicit export
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")

# Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_DLL") # TODO
  
# Packages
find_package(Boost REQUIRED)

# Sources
file(GLOB_RECURSE libpowerwin_SOURCES "*.cpp")

# Includes / Libs
include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

# Target 32bit
set(RAW_RC_FLAGS "${CMAKE_RC_FLAGS}")

set_source_files_properties(powerwin32.rc PROPERTIES COMPILE_FLAGS "--output-format=coff --target=pe-i386")
add_library(powerwin32 SHARED ${libpowerwin_SOURCES} powerwin32.rc)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set_target_properties(powerwin32 PROPERTIES COMPILE_OPTIONS "-m32" LINK_FLAGS "-m32")
endif()
target_link_libraries(powerwin32 lightports-base32 lightports-extra32 lightports-ipc32 lightports-dll32)

# Target 64bit
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set_source_files_properties(powerwin64.rc PROPERTIES COMPILE_FLAGS "--output-format=coff")
  add_library(powerwin64 SHARED ${libpowerwin_SOURCES} powerwin64.rc)
  target_link_libraries(powerwin64 lightports-base64 lightports-extra64 lightports-ipc64 lightports-dll64)
endif()


