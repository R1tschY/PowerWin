# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# TODO. add options: INCLUDES LIBRARIES DEFINITIONS
macro(powerwin_module)
  
  # options
  
  set(options )
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES HOOKSOURCES DEPS HOOKDEPS)
  set(prefix powerwin_module)
  cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  
  # debug
  message(STATUS "Found module: ${${prefix}_NAME}")
  
  # main modules
  if (${prefix}_SOURCES AND CMAKE_SIZEOF_VOID_P EQUAL 4)
    add_library("powerwin-${${prefix}_NAME}" STATIC ${${prefix}_SOURCES})
    target_link_libraries("powerwin-${${prefix}_NAME}" ${PowerWin_LINKER_FLAGS} cpp-utils lightports-core ${${prefix}_DEPS})
    
    if (STATIC_QT)
        set_target_properties("powerwin-${${prefix}_NAME}" PROPERTIES
            LINK_SEARCH_START_STATIC ON
            LINK_SEARCH_END_STATIC ON
        )
    endif()
    
    list(APPEND PowerWinApp_MODULES "powerwin-${${prefix}_NAME}")
    move_to_parent(PowerWinApp_MODULES)
    
    list(APPEND PowerWinApp_TRANSLATION_SOURCES ${${prefix}_SOURCES})
    move_to_parent(PowerWinApp_TRANSLATION_SOURCES)
  endif()
  
  # hook modules
  if (${prefix}_HOOKSOURCES)
    set(_module_lib_name "powerwin-hook-${${prefix}_NAME}")
    add_library("${_module_lib_name}" STATIC ${${prefix}_HOOKSOURCES})
    target_link_libraries("${_module_lib_name}" ${PowerWin_LINKER_FLAGS} cpp-utils lightports-core ${${prefix}_HOOKDEPS})
    list(APPEND libpowerwin_MODULES "${_module_lib_name}")
    move_to_parent(libpowerwin_MODULES)
  endif()
  
endmacro()

# include all module subdirectories

file(GLOB _subdirs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*")
foreach(_subdir ${_subdirs})
  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_subdir}")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_subdir}/deactivated_tag")
    else()
        add_subdirectory("${_subdir}")
    endif()
  endif()
endforeach()

move_to_parent(PowerWinApp_MODULES)
move_to_parent(libpowerwin_MODULES)
move_to_parent(PowerWinApp_TRANSLATION_SOURCES)
