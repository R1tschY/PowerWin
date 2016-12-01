# Sources
file(GLOB_RECURSE PowerWinApp_SOURCES "*.cpp")
file(GLOB_RECURSE PowerWinApp_RESOURCES "*.rc")

# Libs
set(PowerWinApp_Libs 
  cpp-utils
  lightports-base 
  lightports-extra 
  lightports-shell 
  lightports-com 
  lightports-controls
  lightports-gdi-controls
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

if (COMPILER_PREFIX)
  set(CMAKE_SIZE "${COMPILER_PREFIX}-size")
else()
  set(CMAKE_SIZE "size")
endif()

add_custom_command(TARGET PowerWinApp
                   POST_BUILD
                   COMMAND ${CMAKE_SIZE} -t "../bin/*.exe" "../bin/*.dll"
                   COMMENT "Sizes:")
