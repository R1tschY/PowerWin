if(BIICODE)
	include(${CMAKE_HOME_DIRECTORY}/biicode.cmake)
     
	# Clear biicode auto detected files.
	# BII_LIB_TARGET will be an interface target.
	SET(BII_LIB_SRC)
	SET(BII_BLOCK_EXES) 
   
	ADD_BII_TARGETS()
	include(biicode/boost/setup)
else()
	cmake_minimum_required (VERSION 2.8.11)
	project (PowerWin)
endif()

if (NOT(CMAKE_SIZEOF_VOID_P EQUAL 8))
  message(WARNING "You need a 64bit enviroment to compile the 64bit hook library.")
endif()

# Boost options
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON) 

# C++14
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

# Windows
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--subsystem,windows")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--subsystem,windows")

# Unicode
add_definitions(-DUNICODE -D_UNICODE)

# Windows XP
#add_definitions(-DNTDDI_VERSION=0x05010300 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -D_WIN32_IE=0x0700)

# Windows Vista
add_definitions(-DNTDDI_VERSION=0x06000000 -D_WIN32_WINNT=0x0600 -DWINVER=0x0600 -D_WIN32_IE=0x0700)

# Debug mode
set(DEBUG_FLAGS "-Og -DDEBUG")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${DEBUG_FLAGS}")

# Release mode
set(RELEASE_FLAGS "-flto -s -O2 -DNDEBUG")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${RELEASE_FLAGS}")

# Warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
 -Wall \
 -Wextra \
 -Wnon-virtual-dtor \
 -Wcast-align \
 -Wunused \
 -Woverloaded-virtual \
 -Wcast-qual \
 -Wconversion \
")

# -Wold-style-cast -Wzero-as-null-pointer-constant -Wshadow

# Output directory
if(NOT BIICODE)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} libs)

add_subdirectory(hooklib)
add_subdirectory(app)

# utils
add_custom_target(update COMMAND cmake .)
