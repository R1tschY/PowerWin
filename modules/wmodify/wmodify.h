///
/// Copyright (c) 2017 R1tschY
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#pragma once

#include <QWidget>
#include <windows.h>

#include <app/module.h>
#include <app/helper/observermixin.h>
#include <lightports/user/geometry.h>
#include <lightports/controls/control.h>

namespace PowerWin {

class WModifyOverlay : public QWidget
{
  Q_OBJECT
public:
  WModifyOverlay();

private:

};

class WModify : public Module, PowerWin::ObserverMixin
{
public:
  WModify(ModuleContext& context);

private:
  enum class State {
    Idle,
    Moving,
    Resizing
  } state_ = State::Idle;


  bool handleButtonDown(Windows::Point pt, int button);
  bool handleMouseMove(Windows::Point pt);
  bool handleButtonUp(Windows::Point pt, int button);

  Windows::Point initial_pt_;
  Windows::Window hwnd_;
  Windows::Rectangle inital_rect_;

  // Resizing
  int vresize_ = 0;
  int hresize_ = 0;
};

} // namespace PowerWin
