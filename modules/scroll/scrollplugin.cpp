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

#include <functional>
#include <cpp-utils/algorithm/container.h>
#include <lightports/core.h>
#include <lightports/user/application.h>
#include <lightports/controls/control.h>
#include <modules/scroll/scrollplugin.h>
#include <app/mousehook.h>

using namespace Windows;

ScrollPlugin::ScrollPlugin(PowerWin::ModuleContext& context)
{
  auto& config = context.getConfiguration();
  inverse_ = config.readBoolean(L"scroll", L"inverse", false);

  connect(context.getMouseHook().mouseWheel(),
    [=](POINT pt, int steps, DWORD time){ return handle(pt, steps, time); });
}

__attribute__((hot))
bool ScrollPlugin::handle(POINT pt, int steps, DWORD time)
{
  // get window under cursor
  Window window = Window::at(pt);
  if (window == NULL)
  {
    ::OutputDebugStringW(L"ScrollPlugin: window == NULL\n");
    return false;
  }

  if (inverse_)
  {
    steps = -steps;
  }
    
  int vkeys = 
      ((::GetKeyState(VK_SHIFT) & SHIFTED) != 0) << 2
    | ((::GetKeyState(VK_CONTROL) & SHIFTED) != 0) << 3;

  ::PostMessageW(window.getHWND(), WM_MOUSEWHEEL, MAKELONG(vkeys, steps),
      MAKELONG(pt.x, pt.y));

  return true;
}

PowerWin::ModuleRegistry::element<ScrollPlugin> ScrollModule(
  L"scroll", L"scroll in inactive windows"
);
