
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
  if (${prefix}_SOURCES)
    add_library("powerwin-${${prefix}_NAME}" STATIC ${${prefix}_SOURCES})
    set_target_properties("powerwin-${${prefix}_NAME}" PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
    target_link_libraries("powerwin-${${prefix}_NAME}" cpp-utils32 lightports-base32 ${${prefix}_DEPS})
    
    list(APPEND PowerWinApp_MODULES "powerwin-${${prefix}_NAME}")
    move_to_parent(PowerWinApp_MODULES)
  endif()
  
  # hook modules
  if (${prefix}_HOOKSOURCES)
    # 32bit module
    add_library("powerwin-${${prefix}_NAME}32" STATIC ${${prefix}_HOOKSOURCES})
    set_target_properties("powerwin-${${prefix}_NAME}32" PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
    target_link_libraries("powerwin-${${prefix}_NAME}32" cpp-utils32 lightports-base32 ${${prefix}_DEPS})
    list(APPEND libpowerwin_MODULES32 "powerwin-${${prefix}_NAME}32")  
    move_to_parent(libpowerwin_MODULES32)
  
    # 64bit module
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      add_library("powerwin-${${prefix}_NAME}64" STATIC ${${prefix}_HOOKSOURCES})
      target_link_libraries("powerwin-${${prefix}_NAME}64" cpp-utils64 lightports-base64 ${${prefix}_DEPS})
      list(APPEND libpowerwin_MODULES64 "powerwin-${${prefix}_NAME}64")
      move_to_parent(libpowerwin_MODULES64)
    endif()
  endif()
  
endmacro()

# include all module subdirectories

file(GLOB _subdirs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*")
foreach(_subdir ${_subdirs})
  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_subdir}")
    add_subdirectory("${_subdir}")
  endif()
endforeach()

move_to_parent(PowerWinApp_MODULE_SOURCES)
move_to_parent(libpowerwin_MODULE_SOURCES)
