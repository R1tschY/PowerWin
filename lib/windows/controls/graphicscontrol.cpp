#include "graphicscontrol.h"

#include <uxtheme.h>
#include "../debug.h"
#include "../memory.h"
#include "gdipluscontext.h"

namespace Windows {

namespace {

#if WINVER >= 0x06000
void EndBufferedPainting(HPAINTBUFFER paint_buffer) {
  BufferedPaintMakeOpaque(paint_buffer, nullptr);
  EndBufferedPaint(paint_buffer, true);
}
WINDOWS_DEFINE_HANDLE_TYPE(PaintBuffer, HPAINTBUFFER, EndBufferedPainting);
#endif

}

LRESULT GraphicsControl::onMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg) {
  case WM_CREATE:  {
#if WINVER >= 0x06000
    HRESULT hr = win_check(BufferedPaintInit());
    double_buffered_ = (hr == S_OK);
#else
    double_buffered_ = false;
#endif
    break;
  }

  case WM_ACTIVATE:
    InvalidateRect(getNativeHandle(), nullptr, true);
    return DefWindowProc(getNativeHandle(), msg, wparam, lparam);

  case WM_ERASEBKGND:
    // Don't do any erasing here.  It's done in WM_PAINT to avoid flicker.
    return 1;

  case WM_PAINT:
    doPaint();
    return 0;

  case WM_PRINTCLIENT: // for AnimateWindow
    doPrintClient(reinterpret_cast<HDC>(wparam));
    return 0;

  case WM_NCDESTROY:
#if WINVER >= 0x06000
    if (double_buffered_)
      win_check(BufferedPaintUnInit());
#endif
    break;
  }

  return base::onMessage(msg, wparam, lparam);
}

void GraphicsControl::onPaint(Gdiplus::Graphics&) { }

void GraphicsControl::doPaint()
{
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(getNativeHandle(), &ps);

#if WINVER >= 0x06000
  PaintBuffer paint_buffer;
  if (double_buffered_) {
    // Get doublebuffered DC
    RECT rc = getClientRect();

    HDC hdc_tmp;
    paint_buffer.reset(BeginBufferedPaint(hdc, &rc, BPBF_COMPOSITED, nullptr, &hdc_tmp));
    if (paint_buffer) {
      hdc = hdc_tmp;
    }
  }
#endif

  Gdiplus::Graphics graphics(hdc);
  onPaint(graphics);

  EndPaint(getNativeHandle(), &ps);
}

void GraphicsControl::doPrintClient(HDC hdc)
{
  Gdiplus::Graphics graphics(hdc);
  onPaint(graphics);
}

} // namespace Windows
