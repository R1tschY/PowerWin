# Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_DLL") # TODO
  
# Packages
include_directories(${Boost_INCLUDE_DIRS}) # XXX

# Sources
file(GLOB_RECURSE libpowerwin_SOURCES "*.cpp")

# Includes / Libs
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(_target "powerwin64")
else()
  set(_target "powerwin32")
endif()

# libpowerwin??
add_library(${_target} SHARED ${libpowerwin_SOURCES})

target_link_libraries(
    ${_target} 
    cpp-utils 
    lightports-core lightports-controls lightports-user lightports-dll
    -Wl,--whole-archive ${libpowerwin_MODULES} -Wl,--no-whole-archive)
    
add_windows_versioninfo(
  TARGET ${_target} 
  ${PowerWin_VERSIONINFO_ARGS})

#install(TARGETS powerwin32 RUNTIME DESTINATION ".")


