
# TODO. add options: INCLUDES LIBRARIES DEFINITIONS
macro(powerwin_module)
  
  # options
  
  set(options )
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES HOOKSOURCES DEPS)
  set(prefix powerwin_module)
  cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  
  # debug
  message(STATUS "Found module: ${${prefix}_NAME}")
  
  # main modules
  if (${prefix}_SOURCES AND CMAKE_SIZEOF_VOID_P EQUAL 4)
    add_library("powerwin-${${prefix}_NAME}" STATIC ${${prefix}_SOURCES})
    target_link_libraries("powerwin-${${prefix}_NAME}" cpp-utils lightports-core ${${prefix}_DEPS})
    
    list(APPEND PowerWinApp_MODULES "powerwin-${${prefix}_NAME}")
    move_to_parent(PowerWinApp_MODULES)
  endif()
  
  # hook modules
  if (${prefix}_HOOKSOURCES)
    set(_module_lib_name "powerwin-hook-${${prefix}_NAME}")
    add_library("${_module_lib_name}" STATIC ${${prefix}_HOOKSOURCES})
    target_link_libraries("${_module_lib_name}" cpp-utils lightports-core ${${prefix}_DEPS})
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
