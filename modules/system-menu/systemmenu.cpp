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

#include "systemmenu.h"

#include <lightports/core/debugstream.h>
#include <lightports/core/debug.h>
#include <lightports/controls.h>
#include <lightports/user/menu.h>
#include <app/helper/toqt.h>

#include "common.h"

using namespace PowerWin;
using namespace Windows;

namespace {

static
bool IsWindowAlwaysOnTop(HWND hwnd) {
  return (GetWindowLongPtr(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
}

static
void updateSystemMenu(HWND hwnd, bool new_state)
{
  MenuView menu = getSystemMenu(hwnd);
  if (menu)
  {
    menu.modifyEntry(
      SystemMenu::AlwaysOnTop,
      to<cpp::wstring_view>(SystemMenuModule::tr("Always on top")),
      new_state ? MenuEntryFlags::Checked : MenuEntryFlags::Unchecked);
  }
}

static
void SetWindowAlwaysOnTop(HWND hwnd, bool new_state)
{
  auto exstyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
  HWND z_pos;

  if (new_state)
  {
    exstyle |= WS_EX_TOPMOST;
    z_pos = HWND_TOPMOST;
  }
  else
  {
    exstyle &= ~WS_EX_TOPMOST;
    z_pos = HWND_NOTOPMOST;
  }

  SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstyle);
  SetWindowPos(hwnd, z_pos, 0,0,0,0,
               SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

static
BOOL CALLBACK systemmenu_upgrade_window(HWND hwnd, LPARAM lParam)
{
  try
  {
    MenuView menu = getSystemMenu(hwnd);
    if (menu.getHMENU())
    {
      // window with system menu

      if (menu.existsEntry(SC_CLOSE))
      {
        // add own menu

        // 'Topmost' entry
        menu.insertEntryBefore(
          SC_CLOSE,
          SystemMenu::AlwaysOnTop,
          to<cpp::wstring_view>(SystemMenuModule::tr("Always on top")),
          IsWindowAlwaysOnTop(hwnd)
          ? MenuEntryFlags::Checked : MenuEntryFlags::Unchecked);

        // seperator
        menu.insertSeperatorBefore(SC_CLOSE, SystemMenu::AlwaysOnTop);
      }
    }
  }
  catch (...)
  {
    ::OutputDebugStringW(L"PowerWin system menu module: exception in systemmenu_upgrade_window");
  }

  return true;
}

static
BOOL CALLBACK downgradeWindow(HWND hwnd, LPARAM lParam)
{
  try
  {
    MenuView menu = getSystemMenu(hwnd);
    if (menu) {
      // remove existing menu
      int i = 2;
      while (menu.existsEntry(SystemMenu::AlwaysOnTop) && i--)
        menu.deleteEntry(SystemMenu::AlwaysOnTop);
    }
  }
  catch (...)
  {
    ::OutputDebugStringW(L"PowerWin system menu module: exception in downgradeWindow");
  }

  return true;
}

}  // namespace

SystemMenuModule::SystemMenuModule(ModuleContext& context)
{
  new_window_msg_ = win_throw_on_fail(
      ::RegisterWindowMessageW(SystemMenu::NewWindowMessage));
  toggle_topmost_msg_ = win_throw_on_fail(
      ::RegisterWindowMessageW(SystemMenu::SetTopmostMessage));

  auto& events = context.getGlobalEventsManager();
  ObserverMixin::connect(
      events.getWindowsMessageSignal(),
      [=](WindowsMessageEvent& e) { onGlobalMessage(e); });

  EnumWindows(downgradeWindow, 0);
  EnumWindows(systemmenu_upgrade_window, 0);
}

SystemMenuModule::~SystemMenuModule()
{
  EnumWindows(downgradeWindow, 0);
}

void SystemMenuModule::onGlobalMessage(WindowsMessageEvent& e)
{
  if (e.getMessage() == new_window_msg_)
  {
    onNewWindow(reinterpret_cast<HWND>(e.getWParam()));
    e.setResult(1);
  }
  else if (e.getMessage() == toggle_topmost_msg_)
  {
    onToogleTopmost(reinterpret_cast<HWND>(e.getWParam()));
    e.setResult(1);
  }
}

void SystemMenuModule::onNewWindow(HWND hwnd)
{
  systemmenu_upgrade_window(hwnd, 0);
}

void SystemMenuModule::onToogleTopmost(HWND hwnd)
{
  bool state = IsWindowAlwaysOnTop(hwnd);
  SetWindowAlwaysOnTop(hwnd, !state);
  updateSystemMenu(hwnd, !state);
}


ModuleRegistry::element<SystemMenuModule> XSystemMenuModule(
  L"system-menu", L"adds extra system menu items to all windows"
);
