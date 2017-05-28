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

#include <app/hotkeymanager.h>
#include <app/module.h>
#include <app/mousehook.h>
#include <app/signal.h>
#include <lightports/user/geometry.h>

namespace PowerWin {

/// \brief
class WKill : public Module
{
public:

  WKill(ModuleContext& context);

private:
  enum class State {
    Idle, Choose
  };

  MouseHook& mouse_hook_;
  Hotkey hotkey_;
  ScopedSignalConnection hook_connection_;
  UINT mouse_control_msg_ = 0;
  HookLibManager& hook_libs_;

  State state_;

  void onHotkey();
  bool onClick(Windows::Point pt, int button, DWORD time);
  bool onMove(Windows::Point pt, DWORD time);

  void startCursorControl();
  void stopCursorControl();
};

} // namespace PowerWin
