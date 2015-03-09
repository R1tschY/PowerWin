#include "onscreendisplay.h"

using namespace Gdiplus;

namespace Windows {

OnScreenDisplay::OnScreenDisplay() :
  GraphicsControl(
    WS_POPUP,
    WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_NOACTIVATE)
{
  // transparent for clicks, ... : WS_EX_TRANSPARENT
}

void OnScreenDisplay::onPaint(Graphics& context)
{
  Pen pen(Color(Color::Black));
  context.DrawRectangle(&pen, getClientRect());
}

} // namespace Windows
