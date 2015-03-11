#ifndef GRAPHICSCONTROL_H
#define GRAPHICSCONTROL_H

#include <windows.h>
#include <gdiplus.h>

#include "../control.h"

namespace Windows {

class GraphicsControl : public Control
{
public:
  // ctors
  explicit GraphicsControl() :
    Control()
  { }

  explicit GraphicsControl(DWORD style, DWORD exstyle = 0) :
    Control(style, exstyle)
  { }

  explicit GraphicsControl(const wchar_t* window_class, DWORD style, DWORD exstyle) :
    Control(window_class, style, exstyle)
  { }

  explicit GraphicsControl(ATOM window_class, DWORD style, DWORD exstyle) :
    Control(window_class, style, exstyle)
  { }
  
  HDC getGraphicsContext() const {
    return GetDC(getNativeHandle());
  }

  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;

  // virtual
  virtual void onPaint(Gdiplus::Graphics& context);

  // properties
  bool isDoubleBuffered() const { return double_buffered_; }
  
private:
  using base = Control;

  void doPaint();
  void doPrintClient(HDC hdc);

  bool double_buffered_ = false;
};

} // namespace Windows

#endif // GRAPHICSCONTROL_H
