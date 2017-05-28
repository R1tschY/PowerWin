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

#include <algorithm>
#include <lightports/user/hook.h>
#include <lightports/user/geometry.h>

#include "signal.h"

namespace PowerWin {

/// \brief
class MouseHook
{
private:
  struct ChainOfResponsibility {
    using result_type = bool;

    template<typename InputIterator>
    bool operator()(InputIterator first, InputIterator last) const
    {
      return std::any_of(first, last, [](auto x) { return bool(x); });
    }
  };

public:
  using MouseButtonSignal = boost::signals2::signal<bool(Windows::Point, int, DWORD), ChainOfResponsibility>;
  using MouseWheelSignal = boost::signals2::signal<bool(Windows::Point, int, DWORD), ChainOfResponsibility>;
  using MouseMoveSignal = boost::signals2::signal<bool(Windows::Point, DWORD), ChainOfResponsibility>;

  static MouseHook& getInstance();

  void activate();
  void deactivate();

  SignalRegisterInterface<MouseMoveSignal> mouseMove() { return mouse_move_; }
  SignalRegisterInterface<MouseButtonSignal> mouseButtonUp() { return mouse_button_up_; }
  SignalRegisterInterface<MouseButtonSignal> mouseButtonDown() { return mouse_button_down_; }
  SignalRegisterInterface<MouseWheelSignal> mouseWheel() { return mouse_wheel_; }

private:
  Windows::Hook hook_;

  // signals
  MouseButtonSignal mouse_button_down_;
  MouseButtonSignal mouse_button_up_;
  MouseWheelSignal mouse_wheel_;
  MouseMoveSignal mouse_move_;

  MouseHook();

  static LRESULT CALLBACK hookProc(int code, WPARAM wparam, LPARAM lparam);
  bool handle(DWORD msg, const MSLLHOOKSTRUCT& data);
  bool handleMouseWheel(const MSLLHOOKSTRUCT& data);
  bool handleButtonUp(const MSLLHOOKSTRUCT& data, int button);
  bool handleButtonDown(const MSLLHOOKSTRUCT& data, int button);
  bool handleMouseMove(const MSLLHOOKSTRUCT& data);
};

} // namespace PowerWin
