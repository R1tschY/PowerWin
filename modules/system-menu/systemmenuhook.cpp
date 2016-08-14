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


namespace {

using namespace Windows;

WINDOWS_DLL_SHARED cpp::uninitialized<Hook> systemmenu_hook;

/*static std::wstring GetMenuItemDisplayName(HMENU menu, unsigned item) {
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  std::wstring result;

  // Hole Typ mit Id
  info.fMask = MIIM_ID;
  info.wID = 0;
  GetMenuItemInfo(menu, item, true, &info);
  result.append(boost::str(boost::wformat(L"ID 0x%08x: ") % info.wID ).c_str());

  // Hole Typ mit Daten
  info.fMask = MIIM_TYPE;
  info.fType = 0;
  info.dwTypeData = NULL;
  GetMenuItemInfo(menu, item, true, &info);
//  result.append(boost::str(boost::wformat(L"0x%08x ") % info.fType ).c_str());

  if (info.fType & MFT_BITMAP)
    result.append(L"Bitmap ");

  if (info.fType & MFT_SEPARATOR)
    result.append(L"Separator ");

  if (info.fType & MFT_OWNERDRAW)
    result.append(L"OwnerDraw ");

  if (info.fType & MFT_RADIOCHECK)
    result.append(L"RadioCheck ");

  if (info.cch != 0) {
    wchar_t buffer[info.cch + 1];
    info.fMask = MIIM_TYPE;
    info.dwTypeData = buffer;
    info.cch++;
    GetMenuItemInfo(menu, item, true, &info);
    result.append(L"String(");
    result.append(buffer);
    result.append(L") ");
  }

  return result;
}*/

const wchar_t* getTopMostString() {
  static std::wstring topmost_string = Resources::getString(POWERWIN_STR_TOPMOST);
  return topmost_string.c_str();
}

bool IsWindowAlwaysOnTop(HWND hwnd) {
  return (GetWindowLongPtr(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
}

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

void updateSystemMenu(HWND hwnd, bool new_state)
{
  Menu menu = Menu::getSystemMenu(hwnd);
  if (menu)
  {
    menu.modifyEntry(
      SystemMenuHook::MenuId::AlwaysOnTop,
      getTopMostString(),
      new_state ? MenuEntryFlags::Checked : MenuEntryFlags::Unchecked);
  }
}

static BOOL CALLBACK downgradeWindow(HWND hwnd, LPARAM lParam)
{
  Menu menu = Menu::getSystemMenu(hwnd);
  if (menu) {
    // remove existing menu
    if (menu.existsEntry(SystemMenuHook::MenuId::AlwaysOnTop))
      menu.deleteEntry(SystemMenuHook::MenuId::AlwaysOnTop);
  }

  return true;
}

BOOL CALLBACK systemmenu_upgrade_window(HWND hwnd, LPARAM lParam)
{
  Menu menu = Menu::getSystemMenu(hwnd);
  if (menu)
  {
    // window with system menu

    if (menu.existsEntry(SC_CLOSE))
    {
      // add own menu

      // 'Topmost' entry
      menu.insertEntryBefore(
        SC_CLOSE,
        SystemMenuHook::MenuId::AlwaysOnTop,
        getTopMostString(),
        IsWindowAlwaysOnTop(hwnd)
        ? MenuEntryFlags::Checked : MenuEntryFlags::Unchecked);

      // seperator
      menu.insertSeperatorBefore(SC_CLOSE);
    }
  }

  return true;
}

LRESULT CALLBACK systemmenu_hook_proc(int code, WPARAM wParam, LPARAM lParam)
{
  if (code >= 0)
  {
    if (code == HCBT_SYSCOMMAND &&
        wParam == SystemMenuHook::MenuId::AlwaysOnTop)
    {
      // menu item was clicked
      MessageBeep(MB_ICONINFORMATION);
      auto hwnd = GetForegroundWindow();
      bool state = IsWindowAlwaysOnTop(hwnd);
      SetWindowAlwaysOnTop(hwnd, !state);
      updateSystemMenu(hwnd, !state);
      return 1;
    }

    if (code == HCBT_CREATEWND) {
      // new window was createded
      systemmenu_upgrade_window((HWND)wParam, 0);
    }
  }

  return systemmenu_hook->callNext(code, wParam, lParam);
}

}

SystemMenuHook::SystemMenuHook(PowerWin::HookModuleContext& context)
: hook_(systemmenu_hook)
{
  hook_->create(WH_CBT, Hook::AllThreads, systemmenu_hook_proc);

#if CPUBITSET == 32
  MessageBeep(MB_OK);
  EnumWindows(downgradeWindow, 0);
  EnumWindows(systemmenu_upgrade_window, 0);
#endif
}

SystemMenuHook::~SystemMenuHook()
{
#if CPUBITSET == 32
  MessageBeep(MB_OK);
  EnumWindows(downgradeWindow, 0);
#endif
}

PowerWin::HookModuleRegistry::element<SystemMenuHook> SystemMenuHookModule(
  L"system-menu", L"adds extra system menu items to all windows"
);
