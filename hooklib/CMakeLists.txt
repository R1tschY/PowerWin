# Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_DLL") # TODO
  
# Packages
include_directories(${Boost_INCLUDE_DIRS})

# Sources
file(GLOB_RECURSE libpowerwin_SOURCES "*.cpp")

# Includes / Libs
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(libpowerwin_Libs 
  lightports-base
  lightports-extra
  lightports-controls
  lightports-dll
)

# libpowerwin32
set_source_files_properties(powerwin32.rc PROPERTIES COMPILE_FLAGS "--output-format=coff --target=pe-i386")
add_library(powerwin32 SHARED ${libpowerwin_SOURCES} powerwin32.rc)
set_target_properties(powerwin32 PROPERTIES COMPILE_OPTIONS "-m32" LINK_FLAGS "-m32")

listx_append_suffix(libpowerwin_Libs32 32 ${libpowerwin_Libs})
target_link_libraries(powerwin32 cpp-utils32 ${libpowerwin_Libs32} -Wl,--whole-archive ${libpowerwin32_MODULES} -Wl,--no-whole-archive)
add_windows_versioninfo(
  TARGET powerwin32 
  ${PowerWin_VERSIONINFO_ARGS} 
  RC_FILE libpowerwin32_VERSIONINFO_RC)
set_source_files_properties(${libpowerwin32_VERSIONINFO_RC} PROPERTIES COMPILE_FLAGS "--output-format=coff --target=pe-i386")

install(TARGETS powerwin32 RUNTIME DESTINATION ".")

# libpowerwin64
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set_source_files_properties(powerwin64.rc PROPERTIES COMPILE_FLAGS "--output-format=coff")
  add_library(powerwin64 SHARED ${libpowerwin_SOURCES} powerwin64.rc)
  listx_append_suffix(libpowerwin_Libs64 64 ${libpowerwin_Libs})
  target_link_libraries(powerwin64 cpp-utils64 ${libpowerwin_Libs64} -Wl,--whole-archive ${libpowerwin64_MODULES} -Wl,--no-whole-archive)
  add_windows_versioninfo(
    TARGET powerwin64
    ${PowerWin_VERSIONINFO_ARGS} 
    RC_FILE libpowerwin64_VERSIONINFO_RC)
  set_source_files_properties(${libpowerwin64_VERSIONINFO_RC} PROPERTIES COMPILE_FLAGS "--output-format=coff")
  
  install(TARGETS powerwin64 RUNTIME DESTINATION ".")
endif()


