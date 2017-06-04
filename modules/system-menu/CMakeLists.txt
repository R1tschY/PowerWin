powerwin_module(
  NAME system-menu
  SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/systemmenu.cpp"
  HOOKSOURCES "${CMAKE_CURRENT_SOURCE_DIR}/systemmenuhookmodule.cpp"
  DEPS lightports-core Qt5::Core
  HOOKDEPS lightports-core
)