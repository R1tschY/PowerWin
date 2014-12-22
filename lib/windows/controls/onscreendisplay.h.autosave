#ifndef ONSCREENDISPLAY_H
#define ONSCREENDISPLAY_H

#include "../control.h"

namespace Windows {

class OnScreenDisplay : Control
{
public:
  enum AnimateMode
  {
      Blend = AW_BLEND,
      SlideRightToLeft = AW_HOR_NEGATIVE | AW_SLIDE,
      SlideLeftToRight = AW_HOR_POSITIVE | AW_SLIDE,
      SlideTopToBottom = AW_VER_POSITIVE | AW_SLIDE,
      SlideBottomToTop = AW_VER_NEGATIVE | AW_SLIDE,
      RollRightToLeft = AW_HOR_NEGATIVE,
      RollLeftToRight = AW_HOR_POSITIVE,
      RollTopToBottom = AW_VER_NEGATIVE,
      RollBottmToTop = AW_VER_POSITIVE,
      ExpandCollapse = AW_CENTER
  }

  OnScreenDisplay();

  void create(int x, int y, int width, int height) {
    create(nullptr, lit(""), x, y, width, height);
  }

  void show() {
    show(SW_SHOWNOACTIVATE);
  }
};

} // namespace Windows

#endif // ONSCREENDISPLAY_H
