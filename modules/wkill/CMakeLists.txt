powerwin_module(
  NAME wkill
  DEPS lightports-user lightports-os Qt5::Widgets
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/wkill.cpp
)