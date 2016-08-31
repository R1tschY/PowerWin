/*
 * systemmenuhook.cpp
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#include "systemmenuhook.h"

#include <windows.h>
#include <cpp-utils/storage/uninitized.h>
#include <lightports/dll/dll.h>
#include <lightports/extra/menu.h>
#include <lightports/base/resources.h>
#include <lightports/core.h>
#include <lightports/core/debugstream.h>
#include <hooklib/macros.h>
#include <hooklib/resources.h>

#include "common.h"

using namespace PowerWin;
using namespace Windows;

namespace {

WINDOWS_DLL_SHARED cpp::uninitialized<Hook> systemmenu_hook;
SystemMenuHook* active_module = nullptr;

LRESULT CALLBACK systemmenu_hook_proc(int code, WPARAM wParam, LPARAM lParam)
{
  if (code >= 0)
  {
    try
    {
      if (code == HCBT_SYSCOMMAND && wParam == SystemMenu::AlwaysOnTop)
      {
        // menu item was clicked
        if (active_module)
          active_module->onToogleTopmost(
              reinterpret_cast<HWND>(::GetForegroundWindow()));
        return 1;
      }

      if (code == HCBT_CREATEWND)
      {
        // new window was created
        if (active_module)
          active_module->onNewWindow(reinterpret_cast<HWND>(wParam));
      }
    }
    catch (...)
    {
      ::OutputDebugStringW(L"PowerWin system menu module: exception in hook function");
    }
  }

  return systemmenu_hook->callNext(code, wParam, lParam);
}

}

SystemMenuHook::SystemMenuHook(HookModuleContext& context)
: hook_(systemmenu_hook)
{
  cpp_assert(active_module == nullptr);
  active_module = this;

  new_window_msg_ = win_throw_on_fail(
      ::RegisterWindowMessageW(SystemMenu::NewWindowMessage));
  set_topmost_msg_ = win_throw_on_fail(
      ::RegisterWindowMessageW(SystemMenu::SetTopmostMessage));
  powerwin_hwnd_ = context.getPowerWinWHND();

  hook_->create(WH_CBT, Hook::AllThreads, systemmenu_hook_proc);
}

SystemMenuHook::~SystemMenuHook()
{
  active_module = nullptr;
}

void SystemMenuHook::onNewWindow(HWND hwnd)
{
  win_print_on_fail(::PostMessageW(
      powerwin_hwnd_,
      new_window_msg_,
      reinterpret_cast<WPARAM>(hwnd), 0));
}

void SystemMenuHook::onToogleTopmost(HWND hwnd)
{
  win_print_on_fail(::PostMessageW(
      powerwin_hwnd_,
      set_topmost_msg_,
      reinterpret_cast<WPARAM>(hwnd), 0));
}


HookModuleRegistry::element<SystemMenuHook> SystemMenuHookModule(
  L"system-menu", L"adds extra system menu items to all windows"
);
