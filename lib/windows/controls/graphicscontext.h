#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include <memory>
#include <windows.h>

#include "../macros.h"

namespace Windows {

class GraphicsContext {
public:
  GraphicsContext(HDC handle) :
    handle_(handle)
  { }

  // properties
  int getDeviceCaps(int index) const { return GetDeviceCaps(handle_, index); }
  double getDPIScale() const { return getDeviceCaps(LOGPIXELSX) / 96.; }

  // manipulators

  //void setBrush() { SelectObject

  HDC getNativeHandle() { return handle_; }

protected:
  HDC handle_;
};

class GraphicsContextHolder : public GraphicsContext {
public:
  // ctors
  GraphicsContextHolder() :
    GraphicsContext(nullptr), hwnd_(nullptr)
  { }

  GraphicsContextHolder(HWND hwnd, HDC hdc) :
    GraphicsContext(hdc), hwnd_(hwnd)
  { }

  ~GraphicsContextHolder()
  {
    remove();
  }

  // no copy
  GraphicsContextHolder(const GraphicsContextHolder&& other) = delete;
  GraphicsContextHolder& operator=(const GraphicsContextHolder& other) = delete;

  // move
  GraphicsContextHolder(GraphicsContextHolder&& other) :
    GraphicsContext(other.handle_), hwnd_(other.hwnd_)
  {
    other.handle_ = nullptr;
  }

  GraphicsContextHolder& operator=(GraphicsContextHolder&& other) {
    remove();
    handle_ = other.handle_;
    other.handle_ = nullptr;
    return *this;
  }

  // reset
  void reset() {
    remove();
    handle_ = nullptr;
  }

private:
  HWND hwnd_;

  void remove() {
    if (handle_)
      ReleaseDC(hwnd_, handle_);
  }
};

} // namespace Windows

#endif // GRAPHICSCONTEXT_H
