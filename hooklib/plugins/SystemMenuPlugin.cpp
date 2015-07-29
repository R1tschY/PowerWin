#include "SystemMenuPlugin.h"

#include <vector>
#include <cstdio>

#include "../windows/debug.h"
#include "../windows/hook.h"
#include "../c++/utils.h"
#include "../c++/uninitized.h"
#include "../macros.h"

///////////////////////////////////////////////////////////////////////////////
// Was noch nicht geht:
//   neu erstellten Fenstern Systemmenü ergänzen
//   64Bit prozesse

static LRESULT CALLBACK systemmenu_cbt_proc(int code, WPARAM wparam, LPARAM lparam);

DLL_SHARED cpp::uninitialized<Windows::Hook> cbt_hook;

SystemMenuPlugin::SystemMenuPlugin() :
  Plugin(L"system_menu")
{
  cbt_hook.construct();
}

static bool ExistsMenuItem(HMENU menu, unsigned id) {
  MENUITEMINFO info;
  info.cbSize = sizeof(info);
  info.fMask = MIIM_ID;
  info.wID = 0;
  return GetMenuItemInfo(menu, id, false, &info) && info.wID == id;
}

static bool IsWindowAlwaysOnTop(HWND hwnd) {
  return (GetWindowLongPtr(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
}

static void SetWindowAlwaysOnTop(HWND hwnd, bool new_state) {
  auto exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);

  if (new_state) {
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstyle | WS_EX_TOPMOST);
    SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
  } else {
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstyle & (~WS_EX_TOPMOST));
    SetWindowPos(hwnd, HWND_NOTOPMOST, 0,0,0,0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
  }
}

static void UpdateSystemMenu(HWND hwnd, bool new_state) {
  HMENU menu = GetSystemMenu(hwnd, false);
  if (IsMenu(menu)) {
    UINT state_flag = ((new_state) ? (MF_CHECKED) : (MF_UNCHECKED));
    ModifyMenu(
          menu,
          SystemMenuPlugin::MenuId_AlwaysOnTop,
          MF_STRING | state_flag,
          SystemMenuPlugin::MenuId_AlwaysOnTop,
          L"Immer im Vordergrund");
  }
}

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

static BOOL CALLBACK upgrade_window(HWND hwnd, LPARAM lParam) {
  wchar_t class_name[255];
  GetClassNameW(hwnd, class_name, cpp::length(class_name));
  HMENU menu = GetSystemMenu(hwnd, false);

  if (IsMenu(menu)) {
    // hat Systemmenu

    if (ExistsMenuItem(menu, SC_CLOSE)) {
      // eigenes Menü hinzufügen

      // Immer im Vordergrund
      InsertMenu(menu, SC_CLOSE,
                 IsWindowAlwaysOnTop(hwnd) ? MF_CHECKED : MF_UNCHECKED,
                 SystemMenuPlugin::MenuId_AlwaysOnTop,
                 L"Immer im Vordergrund");

      // Seperator
      InsertMenu(menu, SC_CLOSE, MF_SEPARATOR, SystemMenuPlugin::MenuId_AlwaysOnTop, nullptr);
    }
  }

  return true;
}

#ifdef MAIN_MODULE
static BOOL CALLBACK downgrade_window(HWND hwnd, LPARAM lParam) {
  HMENU menu = GetSystemMenu(hwnd, false);
  if (IsMenu(menu)) {
    // hat Systemmenu

    // vorhandenes Menü löschen
    while (ExistsMenuItem(menu, SystemMenuPlugin::MenuId_AlwaysOnTop))
      DeleteMenu(menu, SystemMenuPlugin::MenuId_AlwaysOnTop, MF_BYCOMMAND);
  }

  return true;
}
#endif

void SystemMenuPlugin::onActivate(const Plugin::Options& options)
{
  cbt_hook.construct();
  cbt_hook->create(WH_CBT, Windows::Hook::AllThreads, systemmenu_cbt_proc);

#ifdef MAIN_MODULE
  MessageBeep(MB_OK);
  EnumWindows(downgrade_window, 0);
  EnumWindows(upgrade_window, 0);
#endif
}

void SystemMenuPlugin::onDeactivate()
{
#ifdef MAIN_MODULE
  MessageBeep(MB_OK);
  EnumWindows(downgrade_window, 0);
#endif

  cbt_hook->destroy();
}

static
LRESULT CALLBACK systemmenu_cbt_proc(int code, WPARAM wParam, LPARAM lParam) {
  if (code >= 0)
  {
    if (code == HCBT_SYSCOMMAND &&
        wParam == SystemMenuPlugin::MenuId_AlwaysOnTop)
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
      upgrade_window((HWND)wParam, 0);
    }
  }

  return cbt_hook->callNext(code, wParam, lParam);
}
