# explicit export
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")

# Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_DLL")

# Packages
find_package(Boost REQUIRED)

# Sources
file(GLOB_RECURSE libpowerwin_SOURCES "*.cpp")
file(GLOB_RECURSE libpowerwin_HEADERS "*.h")

# Libs
set(libpowerwin_LIBS "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lwinpthread -Wl,-Bdynamic -lgdiplus")

# Target 32bit
add_library(powerwin32 SHARED ${libpowerwin_SOURCES})
target_include_directories(powerwin32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(powerwin32 PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif
target_link_libraries(powerwin32 ${libpowerwin_LIBS})

# Target 64bit
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_library(powerwin64 SHARED ${libpowerwin_SOURCES})
	target_include_directories(powerwin64 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	target_link_libraries(powerwin64 ${libpowerwin_LIBS})
endif()

# Includes / Libs
include_directories(. ${Boost_INCLUDE_DIRS})


