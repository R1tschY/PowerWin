#include "SystemMenuPlugin.h"

#include <vector>
#include <cstdio>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include "../Utils.h"
#include "../Hook.h"
#include "../c++/utils.h"

///////////////////////////////////////////////////////////////////////////////
// Was noch nicht geht:
//   neu erstellten Fenstern Systemmenü ergänzen
//   Auf Klick reagieren
// -> theoretisch reicht CBT-Hook (alle Hooks funktionieren nicht)

LRESULT CALLBACK systemmenu_proc(int code, WPARAM wparam, LPARAM lparam);
static DLL_SHARED Hook hook(WH_GETMESSAGE, Hook::THREAD_ID_ALL_THREADS, systemmenu_proc);

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
  return (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
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
                 SC_CLOSE, //SystemMenuPlugin::MenuId_AlwaysOnTop,
                 L"Immer im Vordergrund");

      // Seperator
      InsertMenu(menu, SC_CLOSE, MF_SEPARATOR, 0, nullptr);
    }
  }

  return true;
}

void SystemMenuPlugin::onActivate(const Plugin::Options& options)
{
  hook.activate();
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
  hook.deactivate();
  EnumWindows(downgrade_window, 0);
}

static int i = 0;
LRESULT CALLBACK systemmenu_proc(int code, WPARAM wparam, LPARAM lparam) {
  if (code >= 0)
  {
    /*CWPSTRUCT* msg = reinterpret_cast<CWPSTRUCT*>(lparam);
    if (msg->message == WM_SYSCOMMAND) {
      print(L"%d: SystemMenu Hook 0x%x 0x%x 0x%x!\n",++i, msg->hwnd,msg->lParam, msg->wParam);
    }*/
    MSG* msg = reinterpret_cast<MSG*>(lparam);
    if (msg->message == WM_SYSCOMMAND) {
      print(L"%d: SystemMenu Hook 0x%x 0x%x 0x%x!\n",++i, msg->hwnd,msg->lParam, msg->wParam);
    }
  }

  return CallNextHookEx(hook.getHandle(), code, wparam, lparam);
}
