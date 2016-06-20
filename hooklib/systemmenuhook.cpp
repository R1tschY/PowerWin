/*
 * systemmenuhook.cpp
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#include <systemmenuhook.h>

#include <windows.h>
#include <c++/uninitized.h>
#include <c++/utils.h>
#include <windows/extra/hook.h>
#include <windows/base/resources.h>
#include <windows/extra/dll.h>
#include <windows/core.h>
#include "macros.h"
#include "resources.h"

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

bool ExistsMenuItem(HMENU menu, unsigned id) {
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_ID;
  info.wID = 0;
  return GetMenuItemInfo(menu, id, false, &info) && info.wID == id;
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

void UpdateSystemMenu(HWND hwnd, bool new_state) {
  HMENU menu = GetSystemMenu(hwnd, false);
  if (IsMenu(menu)) {
    UINT state_flag = ((new_state) ? (MF_CHECKED) : (MF_UNCHECKED));
    ModifyMenu(
          menu,
          SystemMenuHook::MenuId::AlwaysOnTop,
          MF_STRING | state_flag,
          SystemMenuHook::MenuId::AlwaysOnTop,
          getTopMostString());
  }
}

static BOOL CALLBACK downgrade_window(HWND hwnd, LPARAM lParam) {
  HMENU menu = GetSystemMenu(hwnd, false);
  if (IsMenu(menu)) {
    // system menu exists

    // remove existing menu
    if (ExistsMenuItem(menu, SystemMenuHook::MenuId::AlwaysOnTop))
      DeleteMenu(menu, SystemMenuHook::MenuId::AlwaysOnTop, MF_BYCOMMAND);
  }

  return true;
}

BOOL CALLBACK systemmenu_upgrade_window(HWND hwnd, LPARAM lParam)
{
  HMENU menu = GetSystemMenu(hwnd, false);
  if (IsMenu(menu))
  {
    // window with system menu

    if (ExistsMenuItem(menu, SC_CLOSE))
    {
      // add own menu

      // 'Topmost' entry
      InsertMenu(menu, SC_CLOSE,
                 IsWindowAlwaysOnTop(hwnd) ? MF_CHECKED : MF_UNCHECKED,
                     SystemMenuHook::MenuId::AlwaysOnTop,
                     getTopMostString());

      // seperator
      InsertMenu(menu, SC_CLOSE, MF_SEPARATOR, SystemMenuHook::MenuId::AlwaysOnTop, nullptr);
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
      UpdateSystemMenu(hwnd, !state);
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

namespace SystemMenuHook {

void activate(const Windows::IPCData&)
{
  systemmenu_hook.construct();
  systemmenu_hook->create(WH_CBT, Hook::AllThreads, systemmenu_hook_proc);

  print(L"%s: %s", POWERWIN_APP_NAME, __PRETTY_FUNCTION__);

#if CPUBITSET == 32
  MessageBeep(MB_OK);
  EnumWindows(downgrade_window, 0);
  EnumWindows(systemmenu_upgrade_window, 0);
#endif
}

void deactivate(const Windows::IPCData&)
{
  systemmenu_hook->destroy();

#if CPUBITSET == 32
  MessageBeep(MB_OK);
  EnumWindows(downgrade_window, 0);
#endif
}

} // namespace SystemMenuHook
