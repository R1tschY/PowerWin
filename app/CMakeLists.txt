# Sources
file(GLOB_RECURSE PowerWinApp_SOURCES "*.cpp")
file(GLOB_RECURSE PowerWinApp_RESOURCES "*.qrc")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Libs
set(PowerWinApp_Libs 
  cpp-utils
  Qt5::Widgets
  lightports-core
  lightports-user
  lightports-controls
  ${Boost_LIBRARIES}
)

# Target
add_executable(PowerWinApp ${PowerWinApp_SOURCES} ${PowerWinApp_MODULE_SOURCES} ${PowerWinApp_RESOURCES})
target_link_libraries(PowerWinApp ${PowerWinApp_Libs} -Wl,--whole-archive ${PowerWinApp_MODULES} -Wl,--no-whole-archive)
add_windows_versioninfo(TARGET PowerWinApp ${PowerWin_VERSIONINFO_ARGS})
add_windows_manifest(
  TARGET PowerWinApp 
  TEMPLATE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/powerwin.manifest.in"
  ASSEMBLY_VERSION "${PowerWin_VERSION_NOEXT}")
install(TARGETS PowerWinApp RUNTIME DESTINATION ".")
set_property(TARGET PowerWinApp PROPERTY CXX_STANDARD 14)
set_property(TARGET PowerWinApp PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET PowerWinApp PROPERTY AUTORCC ON) # for *.qrc files


if (COMPILER_PREFIX)
  set(CMAKE_SIZE "${COMPILER_PREFIX}-size")
else()
  set(CMAKE_SIZE "size")
endif()
