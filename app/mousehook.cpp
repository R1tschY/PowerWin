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

#include "mousehook.h"

#include <lightports/user/application.h>
#include <lightports/core.h>

using namespace Windows;

namespace PowerWin {

MouseHook::MouseHook() { }

void MouseHook::activate()
{
  hook_.create(
    Application::getHINSTANCE(),
    Hook::MouseLowLevel,
    Hook::AllThreads,
    &MouseHook::hookProc);
}

void MouseHook::deactivate()
{
  hook_.destroy();
}

__attribute__((hot))
bool MouseHook::handle(DWORD msg, const MSLLHOOKSTRUCT& data)
{
  switch(msg)
  {
  case WM_MOUSEMOVE:
    return handleMouseMove(data);

  case WM_MOUSEWHEEL:
    return handleMouseWheel(data);

  case WM_LBUTTONDOWN:
    return handleButtonDown(data, 0);

  case WM_RBUTTONDOWN:
    return handleButtonDown(data, 1);

  case WM_MBUTTONDOWN:
    return handleButtonDown(data, 2);

  case WM_XBUTTONDOWN:
    return handleButtonDown(data, 3 + high_word(data.mouseData));

  case WM_LBUTTONUP:
    return handleButtonUp(data, 0);

  case WM_RBUTTONUP:
    return handleButtonUp(data, 1);

  case WM_MBUTTONUP:
    return handleButtonUp(data, 2);

  case WM_XBUTTONUP:
    return handleButtonUp(data, 3 + high_word(data.mouseData));
  }

  return false;
}

bool MouseHook::handleButtonUp(const MSLLHOOKSTRUCT& data, int button)
{
  return mouse_button_up_(data.pt, button, data.time);
}

bool MouseHook::handleButtonDown(const MSLLHOOKSTRUCT& data, int button)
{
  return mouse_button_down_(data.pt, button, data.time);
}

bool MouseHook::handleMouseWheel(const MSLLHOOKSTRUCT& data)
{
  return mouse_wheel_(data.pt, high_word(data.mouseData), data.time);
}

bool MouseHook::handleMouseMove(const MSLLHOOKSTRUCT& data)
{
  return mouse_move_(data.pt, data.time);
}

MouseHook& MouseHook::getInstance()
{
  static MouseHook instance;
  return instance;
}

__attribute__((hot))
LRESULT CALLBACK MouseHook::hookProc(int code, WPARAM wparam, LPARAM lparam)
{
  MouseHook& self = getInstance();

  if (code != HC_ACTION) {
    return self.hook_.callNext(code, wparam, lparam);
  }

  try
  {
    bool processed = self.handle(
      wparam,
      *reinterpret_cast<const MSLLHOOKSTRUCT*>(lparam)
    );
    if (processed) {
      return 1;
    }
  }
  catch(const std::exception& exp)
  {
    DebugOutputStream() << L"Exception catched while handling mouse hook: "
      << exp.what();
  }
  catch(...)
  {
    DebugOutputStream() << L"Unknown exception catched while handling mouse hook";
  }

  return self.hook_.callNext(code, wparam, lparam);
}

} // namespace PowerWin
