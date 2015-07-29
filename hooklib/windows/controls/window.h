#ifndef WINDOW_H
#define WINDOW_H

#include "../control.h"
#include "../debug.h"

namespace Windows {

class Window : public Control {
public:
  enum class Type : DWORD {
    Normal = WS_OVERLAPPEDWINDOW,
    Popup = WS_POPUP,
  };

  Window(Type type, DWORD exstyle = 0) :
    Control((DWORD)type, exstyle | WS_EX_LAYERED)
  { }

  void create(HWND parent, cpp::wstring_view title) {
    Control::create(parent, title, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
  }

  void setOpacity(float value);
  float getOpacity() const;
};

} // namespace Windows

#endif // WINDOW_H
