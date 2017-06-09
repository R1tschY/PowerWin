powerwin_module(
  NAME wmodify
  DEPS lightports-user lightports-controls Qt5::Widgets user32
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/wmodify.cpp
)