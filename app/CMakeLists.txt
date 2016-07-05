# Sources
file(GLOB_RECURSE PowerWinApp_SOURCES "*.cpp")
file(GLOB_RECURSE PowerWinApp_RESOURCES "*.rc")

# Resource compiler
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --output-format=coff --target=pe-i386")

# Packages
include_directories(${Boost_INCLUDE_DIRS})

# Libs
set(PowerWinApp_Libs 
  powerwin
  lightports-base 
  lightports-extra 
  lightports-shell 
  lightports-com 
  lightports-controls
  lightports-gdi-controls
)
listx_append_suffix(PowerWinApp_Libs 32 ${PowerWinApp_Libs})

# Target
add_executable(PowerWinApp ${PowerWinApp_SOURCES} ${PowerWinApp_MODULE_SOURCES} ${PowerWinApp_RESOURCES})
set_target_properties(PowerWinApp PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
target_link_libraries(PowerWinApp cpp-utils32 range-v3 ${PowerWinApp_Libs} ${PowerWinApp_MODULES})

