# Sources
file(GLOB_RECURSE PowerWinApp_SOURCES "*.c")
file(GLOB_RECURSE PowerWinApp_RESOURCES "*.rc")
file(GLOB_RECURSE PowerWinApp_HEADERS "*.h")

# Resourcecompiler
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --output-format=coff --target=pe-i386")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32 -mwindows -static-libgcc")

# Target
add_executable(PowerWin ${PowerWinApp_SOURCES} ${PowerWinApp_RESOURCES})
target_link_libraries(PowerWin powerwin32)

