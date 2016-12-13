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

#include "windowdebug.h"

#include <functional>
#include <cpp-utils/algorithm/container.h>
#include <lightports/core.h>
#include <lightports/controls/gdiplus.h>

#include <app/hotkeymanager.h>

namespace PowerWin {

void WindowDebugView::onPaint(Gdiplus::Graphics& context)
{
  using namespace Gdiplus;
  using namespace Windows;

  SolidBrush pen(Color::Black);
  context.FillRectangle(&pen, convert_to<Gdiplus::RectF>(getClientRect()));

  SolidBrush white(Color::White);
  Gdiplus::Font font(L"Arial", 32);
  StringFormat format;
  format.SetAlignment(StringAlignmentCenter);

  context.DrawString(L"Window Debug", -1, &font, convert_to<Gdiplus::RectF>(getClientRect()), &format, &white);
}

WindowDebugView::WindowDebugView()
{
}

void WindowDebugView::onCreate()
{
}

WindowDebugModule::WindowDebugModule(ModuleContext& context)
  : show_hotkey_(context.getHotkeyManager()), hide_hotkey_(context.getHotkeyManager())
{
  auto show_shortcut_str = context
    .getConfiguration()
    .readValue(L"window-debug", L"show", L"Ctrl+Alt+F5");

  auto hide_shortcut_str = context
    .getConfiguration()
    .readValue(L"window-debug", L"hide", L"Shift+Ctrl+Alt+F5");

  show_hotkey_.setCallback([=](){ showView(); });
  show_hotkey_.setKey(show_shortcut_str);

  hide_hotkey_.setCallback([=](){ hideView(); });
  hide_hotkey_.setKey(hide_shortcut_str);
}

WindowDebugModule::~WindowDebugModule()
{
  hideView();
}

void WindowDebugModule::showView()
{

}

void WindowDebugModule::hideView()
{
}


ModuleRegistry::element<WindowDebugModule> WindowDebugX(
  L"window-debug", L"show properties of windows"
);

} // namespace PowerWin
