powerwin_module(
  NAME system-menu
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/SystemMenuPlugin.cpp
  HOOKSOURCES "${CMAKE_CURRENT_SOURCE_DIR}/systemmenuhook.cpp"
)