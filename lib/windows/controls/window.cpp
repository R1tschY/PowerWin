#include "window.h"

namespace Windows {

Window::Window() :
  Control(WS_OVERLAPPEDWINDOW, WS_EX_COMPOSITED | WS_EX_APPWINDOW)
{
}

} // namespace Windows
