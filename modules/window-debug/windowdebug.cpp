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
