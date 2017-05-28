powerwin_module(
  NAME wkill
  DEPS lightports-user lightports-os
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/wkill.cpp
  HOOKSOURCES "${CMAKE_CURRENT_SOURCE_DIR}/wkillhook.cpp"
)