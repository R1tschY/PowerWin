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
  Boost::boost
)

# Translations
set(PowerWinApp_TRANSLATIONS 
    translations/PowerWinApp_en.ts
    translations/PowerWinApp_de.ts
)

configure_file(translations.qrc.in ${CMAKE_BINARY_DIR}/app/translations.qrc COPYONLY)
list(APPEND PowerWinApp_RESOURCES ${CMAKE_BINARY_DIR}/app/translations.qrc)

if (UPDATE_TRANSLATIONS)
    qt5_create_translation(
        PowerWinApp_QM 
        ${PowerWinApp_SOURCES} 
        ${PowerWinApp_TRANSLATIONS}
    )
else()
    qt5_add_translation(PowerWinApp_QM ${PowerWinApp_TRANSLATIONS})
endif()

add_custom_target(translations DEPENDS ${PowerWinApp_QM})

# prevent the generated files from being deleted during make clean
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM true)

# Target
add_executable(PowerWinApp WIN32 ${PowerWinApp_SOURCES} ${PowerWinApp_RESOURCES} ${PowerWinApp_QM})
target_link_libraries(PowerWinApp 
    ${PowerWin_LINKER_FLAGS} 
    ${PowerWinApp_Libs} 
    -Wl,--whole-archive ${PowerWinApp_MODULES} -Wl,--no-whole-archive
)
add_windows_versioninfo(TARGET PowerWinApp ${PowerWin_VERSIONINFO_ARGS})
add_windows_manifest(
  TARGET PowerWinApp 
  TEMPLATE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/powerwin.manifest.in"
  ASSEMBLY_VERSION "${PowerWin_VERSION_NOEXT}")
install(TARGETS PowerWinApp RUNTIME DESTINATION ".")
set_property(TARGET PowerWinApp PROPERTY CXX_STANDARD 14)
set_property(TARGET PowerWinApp PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET PowerWinApp PROPERTY AUTORCC ON) # for *.qrc files

if (STATIC_QT)
    set_target_properties(PowerWinApp PROPERTIES
        LINK_SEARCH_START_STATIC ON
        LINK_SEARCH_END_STATIC ON
    )
endif()
