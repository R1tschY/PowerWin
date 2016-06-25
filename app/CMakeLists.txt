
# Packages
find_package(Boost REQUIRED)

# Sources
file(GLOB_RECURSE PowerWinApp_SOURCES "*.cpp")
file(GLOB_RECURSE PowerWinApp_RESOURCES "*.rc")

# Resource compiler
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --output-format=coff --target=pe-i386")

include_directories(${Boost_INCLUDE_DIRS})

# Target
add_executable(PowerWinApp ${PowerWinApp_SOURCES} ${PowerWinApp_RESOURCES})
set_target_properties(PowerWinApp PROPERTIES COMPILE_FLAGS "-m32 -mwindows" LINK_FLAGS "-m32 -mwindows")
target_link_libraries(PowerWinApp 
  powerwin32 
  lightports-base32 
  lightports-extra32 
  lightports-shell32 
  lightports-com32 
  lightports-controls32
  lightports-gdi-controls32
)

