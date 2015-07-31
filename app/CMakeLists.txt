# Sources
file(GLOB_RECURSE PowerWinApp_SOURCES "*.c")
file(GLOB_RECURSE PowerWinApp_RESOURCES "*.rc")
file(GLOB_RECURSE PowerWinApp_HEADERS "*.h")

# Resourcecompiler
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --output-format=coff --target=pe-i386")

# Target
add_executable(PowerWinApp ${PowerWinApp_SOURCES} ${PowerWinApp_RESOURCES})
set_target_properties(PowerWinApp PROPERTIES COMPILE_FLAGS "-m32 -mwindows -static-libgcc" LINK_FLAGS "-m32 -mwindows -static-libgcc")
target_link_libraries(PowerWinApp powerwin32)

