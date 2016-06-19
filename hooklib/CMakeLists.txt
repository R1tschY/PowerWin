# explicit export
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")

# Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_DLL") # TODO
  
# Packages
find_package(Boost REQUIRED)

# Sources
file(GLOB_RECURSE hooklib_SOURCES "*.cpp")
file(GLOB_RECURSE windows_core_SOURCES "../libs/windows/core/*.cpp")
set(windows_extra_SOURCES ../libs/windows/base/resources.cpp ../libs/windows/extra/hook.cpp ../libs/windows/extra/dll.cpp ../libs/windows/ipc/ipcmailbox.cpp)

set(libpowerwin_SOURCES ${hooklib_SOURCES} ${windows_core_SOURCES} ${windows_extra_SOURCES})

# Libs
set(libpowerwin_LIBS ${libpowerwin_LIBS} ${Boost_LIBRARIES})

# Includes / Libs
include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

# Target 32bit
set(RAW_RC_FLAGS "${CMAKE_RC_FLAGS}")
set_source_files_properties(powerwin32.rc PROPERTIES COMPILE_FLAGS "--output-format=coff --target=pe-i386")
add_library(powerwin32 SHARED ${libpowerwin_SOURCES} powerwin32.rc)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(powerwin32 PROPERTIES COMPILE_OPTIONS "-m32" LINK_FLAGS "-m32")
endif()
target_link_libraries(powerwin32 ${libpowerwin_LIBS})

# Target 64bit
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_source_files_properties(powerwin64.rc PROPERTIES COMPILE_FLAGS "--output-format=coff")
	add_library(powerwin64 SHARED ${libpowerwin_SOURCES} powerwin64.rc)
	target_link_libraries(powerwin64 ${libpowerwin_LIBS})
endif()


