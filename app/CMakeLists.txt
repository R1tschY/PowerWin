
# Packages
find_package(Boost REQUIRED COMPONENTS filesystem system)

# Sources
file(GLOB_RECURSE app_SOURCES "*.cpp")
file(GLOB windows_core_SOURCES "../libs/windows/core/*.cpp")
file(GLOB windows_base_SOURCES "../libs/windows/base/*.cpp")
file(GLOB windows_extra_SOURCES "../libs/windows/extra/*.cpp")
file(GLOB windows_shell_SOURCES "../libs/windows/shell/*.cpp")
file(GLOB windows_com_SOURCES "../libs/windows/com/*.cpp")
file(GLOB windows_controls_SOURCES "../libs/windows/controls/*.cpp")

set(PowerWinApp_SOURCES  ${app_SOURCES} ${windows_core_SOURCES}  ${windows_base_SOURCES}  ${windows_extra_SOURCES} ${windows_shell_SOURCES}  ${windows_com_SOURCES} ${windows_controls_SOURCES} ../libs/windows/ipc/ipcconnection.cpp)

file(GLOB_RECURSE PowerWinApp_RESOURCES "*.rc")

# Resource compiler
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} --output-format=coff --target=pe-i386")

include_directories(${Boost_INCLUDE_DIRS})

# Target
add_executable(PowerWinApp ${PowerWinApp_SOURCES} ${PowerWinApp_RESOURCES})
set_target_properties(PowerWinApp PROPERTIES COMPILE_FLAGS "-m32 -mwindows" LINK_FLAGS "-m32 -mwindows")
target_link_libraries(PowerWinApp powerwin32 -lole32 -lshell32 -lcomctl32 -lgdiplus ${Boost_LIBRARIES})

