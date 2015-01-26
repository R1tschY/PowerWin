#ifndef ONSCREENDISPLAY_H
#define ONSCREENDISPLAY_H

#include "graphicscontrol.h"

#include <c++/stringliteral.h>

namespace Windows {

class OnScreenDisplay : public GraphicsControl
{
public:
  OnScreenDisplay();

  void onPaint(GraphicsContext *context) override;

  void create(int x, int y, int width, int height) {
    Control::create(HWND_TOPMOST, wstring_literal(""), x, y, width, height);
    setPosition(x, y, width, height, SWP_SHOWWINDOW, HWND_TOPMOST);
  }

  void onCreate();
};

} // namespace Windows

#endif // ONSCREENDISPLAY_H
