///
/// Copyright (c) 2016 R1tschY
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

#include <lightports/controls/onscreendisplay.h>
#include <lightports/extra/hook.h>
#include <lightports/base/timeout.h>
#include <app/module.h>
#include <app/hotkeymanager.h>

namespace PowerWin {

class WindowDebugView : public Windows::OnScreenDisplay
{
public:
  WindowDebugView();

  void onPaint(Gdiplus::Graphics& context) override;
  void onCreate() override;

private:
  Windows::Timeout refresh_timeout_;
};

class WindowDebugModule : public Module {
public:
  WindowDebugModule(ModuleContext& context);
  ~WindowDebugModule();

  void showView();
  void hideView();

private:
  WindowDebugView debug_view_;
  Hotkey show_hotkey_;
  Hotkey hide_hotkey_;
};

} // namespace PowerWin

