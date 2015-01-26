#ifndef WINDOW_H
#define WINDOW_H

#include "../control.h"

namespace Windows {

class Window : public Control {
public:
  Window() :
    Control(WS_OVERLAPPEDWINDOW, WS_EX_COMPOSITED | WS_EX_APPWINDOW)
  { }

  void create(HWND parent, cpp::wstring_view title) {
    Control::create(parent, title, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
  }
};

} // namespace Windows

#endif // WINDOW_H
