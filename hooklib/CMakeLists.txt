# explicit export
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")

# Compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_DLL")
  
# Packages
if (BIICODE)
	bii_setup_boost(COMPONENTS filesystem system REQUIRED)
endif()
find_package(Boost COMPONENTS filesystem system REQUIRED)

# Sources
file(GLOB_RECURSE libpowerwin_SOURCES "*.cpp")
file(GLOB_RECURSE libpowerwin_HEADERS "*.h")

# Libs
set(libpowerwin_LIBS "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lwinpthread -Wl,-Bdynamic -lgdiplus -lole32 -lshell32")
if (BIICODE)
	set(libpowerwin_LIBS ${libpowerwin_LIBS} ${BII_LIB_DEPS} ${Boost_LIBRARIES})
else()
	set(libpowerwin_LIBS ${libpowerwin_LIBS} ${Boost_LIBRARIES})
endif()

# Includes / Libs
include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

# Target 32bit
add_library(powerwin32 SHARED ${libpowerwin_SOURCES})
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(powerwin32 PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif()
target_link_libraries(powerwin32 ${libpowerwin_LIBS})

# Target 64bit
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_library(powerwin64 SHARED ${libpowerwin_SOURCES})
	target_link_libraries(powerwin64 ${libpowerwin_LIBS})
endif()

if(BIICODE)
  # If you have configured some file, include the output directory
  # TARGET_INCLUDE_DIRECTORIES(${BII_LIB_TARGET} INTERFACE ${CMAKE_CURRENT_BINARY_DIR})

  # Wire your lib to ${BII_LIB_TARGET} so biicode can use it
  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  	TARGET_LINK_LIBRARIES(${BII_LIB_TARGET} INTERFACE powerwin32 powerwin64)
  else()
	  TARGET_LINK_LIBRARIES(${BII_LIB_TARGET} INTERFACE powerwin32)
  endif()
ENDIF()


