#ifndef WINDOW_H
#define WINDOW_H

#include "../control.h"

namespace Windows {

class Window : public Control {
public:
  Window();

  void create(HWND parent, cpp::wstring_view title) {
    create(parent, title, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
  }
};

} // namespace Windows

#endif // WINDOW_H
