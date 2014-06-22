#include "SystemMenuPlugin.h"

#include <vector>
#include <cstdio>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include "../Utils.h"
#include "../windows/Hook.h"
#include "../c++/utils.h"
#include "../macros.h"

///////////////////////////////////////////////////////////////////////////////
// Was noch nicht geht:
//   neu erstellten Fenstern Systemmenü ergänzen
//   Auf Klick reagieren
// -> theoretisch reicht CBT-Hook (alle Hooks funktionieren nicht)

LRESULT CALLBACK systemmenu_cbt_proc(int code, WPARAM wparam, LPARAM lparam);

static DLL_SHARED Hook cbt_hook(WH_CBT, Hook::THREAD_ID_ALL_THREADS, systemmenu_cbt_proc);

SystemMenuPlugin::SystemMenuPlugin() :
  Plugin(L"system_menu")
{ }

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
    #ifdef _WIN64
      SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstyle | WS_EX_TOPMOST);
    #else
      SetWindowLong(hwnd, GWL_EXSTYLE, exstyle | WS_EX_TOPMOST);
    #endif

    SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
  } else {
    #ifdef _WIN64
      SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstyle & (~WS_EX_TOPMOST));
    #else
      SetWindowLong(hwnd, GWL_EXSTYLE, exstyle & (~WS_EX_TOPMOST));
    #endif

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
  GetClassNameW(hwnd, class_name, Cpp::length(class_name));
  HMENU menu = GetSystemMenu(hwnd, false);

  if (IsMenu(menu)) {
    // hat Systemmenu
    /*wprintf_s(L"Fenster: %s: %p\n", class_name, menu);
    int n = GetMenuItemCount(menu);
    for (int i = 0; i < n; i++) {
      wprintf_s(L"  MenuItem: %s\n", GetMenuItemDisplayName(menu, i).c_str());
    }*/

    if (ExistsMenuItem(menu, SC_CLOSE)) {
      // evtl. noch vorhandenes Menü löschen
      DeleteMenu(menu, SystemMenuPlugin::MenuId_AlwaysOnTop, MF_BYCOMMAND);

      // eigenes Menü hinzufügen

      // Seperator
      InsertMenu(menu, SC_CLOSE, MF_SEPARATOR, 0, nullptr);

      // Immer im Vordergrund
      InsertMenu(menu, SC_CLOSE,
                 IsWindowAlwaysOnTop(hwnd) ? MF_CHECKED : MF_UNCHECKED,
                 SystemMenuPlugin::MenuId_AlwaysOnTop, // SC_CLOSE,
                 L"Immer im Vordergrund");

      // Seperator
      InsertMenu(menu, SC_CLOSE, MF_SEPARATOR, 0, nullptr);
    }
  }

  return true;
}

void SystemMenuPlugin::onActivate(const Plugin::Options& options)
{
  print(L"SystemMenuPlugin::onActivate\n");
  cbt_hook.activate();
  MessageBeep(MB_OK);
  EnumWindows(upgrade_window, 0);
}


static BOOL CALLBACK downgrade_window(HWND hwnd, LPARAM lParam) {
  HMENU menu = GetSystemMenu(hwnd, false);
  if (IsMenu(menu)) {
    // hat Systemmenu

    // vorhandenes Menü löschen
    while (ExistsMenuItem(menu, SystemMenuPlugin::MenuId_AlwaysOnTop))
      DeleteMenu(menu, SystemMenuPlugin::MenuId_AlwaysOnTop, MF_BYCOMMAND);
    InsertMenu(menu, SC_CLOSE, MF_SEPARATOR, 0, nullptr);
  }

  return true;
}

void SystemMenuPlugin::onDeactivate()
{
  cbt_hook.deactivate();
  EnumWindows(downgrade_window, 0);

  MessageBeep(MB_ICONWARNING);
  print(L"SystemMenuPlugin::onDeactivate\n");
}

LRESULT CALLBACK systemmenu_cbt_proc(int code, WPARAM wparam, LPARAM lparam) {
  if (code >= 0)
  {
    if (code == HCBT_SYSCOMMAND) {
      if (wparam == SystemMenuPlugin::MenuId_AlwaysOnTop) {
        MessageBeep(MB_ICONINFORMATION);
        auto hwnd = GetForegroundWindow();
        bool state = IsWindowAlwaysOnTop(hwnd);
        SetWindowAlwaysOnTop(hwnd, !state);
        UpdateSystemMenu(hwnd, !state);
      }
      return 0;
    }
  }

  return CallNextHookEx(cbt_hook.getHandle(), code, wparam, lparam);
}
