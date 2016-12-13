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

#include "systemmenuhookmodule.h"

#include <windows.h>
#include <cpp-utils/storage/uninitized.h>
#include <lightports/dll/dll.h>
#include <lightports/user/menu.h>
#include <lightports/user/resources.h>
#include <lightports/core.h>
#include <lightports/core/debugstream.h>
#include <hooklib/macros.h>

#include "common.h"

using namespace PowerWin;
using namespace Windows;

WINDOWS_DLL_SHARED cpp::uninitialized<SystemMenuHook> hook;

SystemMenuHook::SystemMenuHook(HookModuleContext& context)
{
  cpp_assert(!isAlive());
  alive_ = AliveTag;

  new_window_msg_ = win_throw_on_fail(
      ::RegisterWindowMessageW(SystemMenu::NewWindowMessage));
  set_topmost_msg_ = win_throw_on_fail(
      ::RegisterWindowMessageW(SystemMenu::SetTopmostMessage));
  powerwin_hwnd_ = context.getPowerWinWHND();

  hook_.create(Dll::getHINSTANCE(), WH_CBT, Hook::AllThreads, &SystemMenuHook::onHookMessage);
}

SystemMenuHook::~SystemMenuHook()
{
  alive_ = DeadTag;
  new_window_msg_ = 0;
  set_topmost_msg_ = 0;
  powerwin_hwnd_ = 0;
}

void SystemMenuHook::onNewWindow(HWND hwnd)
{
  // filter windows without system menu
  MenuView menu = getSystemMenu(hwnd);
  if (!menu || !isAlive()) return;

  win_print_on_fail(::PostMessageW(
      powerwin_hwnd_,
      new_window_msg_,
      reinterpret_cast<WPARAM>(hwnd), 0));
}

void SystemMenuHook::onToogleTopmost(HWND hwnd)
{
  if (!isAlive()) return;

  win_print_on_fail(::PostMessageW(
      powerwin_hwnd_,
      set_topmost_msg_,
      reinterpret_cast<WPARAM>(hwnd), 0));
}

LRESULT CALLBACK SystemMenuHook::onHookMessage(int code, WPARAM wParam,
    LPARAM lParam)
{
  if (code >= 0)
  {
    try
    {
      if (code == HCBT_SYSCOMMAND && wParam == SystemMenu::AlwaysOnTop)
      {
        // menu item was clicked
        hook->onToogleTopmost(
              reinterpret_cast<HWND>(::GetForegroundWindow()));
        return 1;
      }

      if (code == HCBT_CREATEWND)
      {
        // new window was created
        hook->onNewWindow(reinterpret_cast<HWND>(wParam));
      }
    }
    catch (...)
    {
      ::OutputDebugStringW(L"PowerWin system menu module: exception in hook function");
    }
  }

  return hook->hook_.callNext(code, wParam, lParam);
}

SystemMenuHookModule::SystemMenuHookModule(HookModuleContext& context)
: hook_(hook, context)
{ }

HookModuleRegistry::element<SystemMenuHookModule> XSystemMenuHookModule(
  L"system-menu", L"adds extra system menu items to all windows"
);
