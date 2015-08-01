#include "window.h"

namespace Windows {

void Window::setOpacity(float value) {
  assert(getNativeHandle());

  win_throw_on_fail(SetLayeredWindowAttributes(getNativeHandle(), RGB(0,0,0), value * 255, LWA_ALPHA));
}

float Window::getOpacity() const {
  assert(getNativeHandle());

  BYTE result;
  DWORD flags;
  win_throw_on_fail(GetLayeredWindowAttributes(getNativeHandle(), nullptr, &result, &flags));
  return (flags & LWA_ALPHA) ? (result / 255.f) : 1.0f;
}

} // namespace Windows
