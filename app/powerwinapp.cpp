#include "powerwinapp.h"

#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <iostream>
#include <thread>

#include <cpp-utils/preprocessor.h>
#include <cpp-utils/strings/string_literal.h>
#include <lightports/core.h>
#include <lightports/controls/utilwindow.h>
#include <lightports/base/path.h>
#include <lightports/base/application.h>
#include <lightports/base/resources.h>
#include <lightports/extra/trayicon.h>
#include <lightports/controls.h>
#include <lightports/base/configfile.h>
#include <lightports/controls/gdipluscontext.h>
#include <lightports/extra/systeminformation.h>
#include <lightports/extra/cursor.h>
#include <lightports/extra/autostart.h>

#include "resources.h"
#include "messages.h"
#include "log.h"
#include "i18n.h"

#include "../hooklib/macros.h"

#include "../hooklib/remotemanager.h"
#include <powerwin-config.h>

using namespace Windows;

namespace PowerWin {

static
ATOM registerWindowClass()
{
  return Windows::Control::registerClass(
      L"PowerWinApp",
      CS_HREDRAW | CS_VREDRAW,
      0,
      LoadCursor(nullptr, IDC_ARROW),
      (HBRUSH)(COLOR_WINDOW+1),
      nullptr,
      0);
}

PowerWinApp::PowerWinApp() :
  MessageSink(registerWindowClass()),
  tray_icon_(),
  configuration_(),
  hotkeys_(),
  global_events_(),
  modules_(configuration_, hotkeys_, global_events_),
  hooklibs_(),
  quit_shortcut_(hotkeys_)
{
}

PowerWinApp::~PowerWinApp() {  }

int PowerWinApp::run()
{
  //  init Comctl32.dll
  /*const INITCOMMONCONTROLSEX icce = {
    sizeof(INITCOMMONCONTROLSEX),
    ICC_STANDARD_CLASSES
  };
  if (!InitCommonControlsEx(&icce)) {
    ERROR(L"%s\n", L"Kann 'common controls' nicht initailsieren!");
  }*/

  Windows::GdiplusContext gdi;

  PowerWinApp powerwin;

  powerwin.create(POWERWIN_PACKAGE_NAME);

  log(Info) << POWERWIN_PACKAGE_NAME << std::hex << L": " << powerwin.getNativeHandle();

  Windows::Application::processMessages();

  log(Info) << POWERWIN_PACKAGE_NAME L": The end" << std::endl;

  return 0;
}

void PowerWinApp::onCreate() {
  print(L"PowerWin::start\n");
  log(Info) << L"ClassName: " << getClassName(getNativeHandle());

  configuration_.loadIniFile(
    Application::getExecutablePath() + L"\\config.ini"
  );

  // tray icon
  tray_icon_.setIcon(POWERWIN_ICON_SMALL);
  tray_icon_.setToolTip(POWERWIN_PACKAGE_NAME);
  tray_icon_.add(getNativeHandle());

  // popup menu
  popup_menu_ = createPopupMenu();

  info_menu_ = createPopupMenu();
  info_menu_.addEntry(InfoEntry, POWERWIN_PACKAGE_NAME POWERWIN_PACKAGE_VERSION);
  info_menu_.addEntry(InfoEntry, _("Copyright © 2014-2016 R1tschY"));
  info_menu_.addEntry(InfoLicence, _("Licenced under GPL v3"));
  popup_menu_.addMenu(L"Info", info_menu_);

  popup_menu_.addSeperator();
  popup_menu_.addEntry(AutostartEntry, _("Start with Windows"));
  popup_menu_.check(AutostartEntry, isProgramInAutostart());
  popup_menu_.addSeperator();
  popup_menu_.addEntry(QuitEntry, _("Quit"));

  // hotkeys
  auto quit_shortcut = configuration_.readValue(L"powerwin", L"quit", L"Ctrl+F12");
  quit_shortcut_.setCallback([=](){ destroy(); });
  quit_shortcut_.setKey(quit_shortcut);

  // local modules
  modules_.loadModules();

  // hook modules
  hooklibs_.startLibs();
}

void PowerWinApp::onDestroy()
{
  log(Info) << L"PowerWinApp::onDestroy\n";

  hooklibs_.unloadLibs();

  // deactivate all plugins
  modules_.unloadModules();

  // remove tray icon
  tray_icon_.remove();

  // exit process
  PostQuitMessage(0);
}

LRESULT PowerWinApp::onMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  // internal management messages
  case Messages::RegisterHooklib:
    hooklibs_.registerHookLib(reinterpret_cast<HWND>(wparam));
    return 0;

  // internal messages
  case WM_NCCREATE:
  case WM_CREATE:
  case WM_DESTROY:
  case WM_NCDESTROY:
    return Control::onMessage(msg, wparam, lparam);

  // filter meaningless messages
  case WM_MOVE:
  case WM_SIZE:
    return ::DefWindowProc(getHWND(), msg, wparam, lparam);

  // trayicon
  case TrayIcon::MessageId:
    return tray_icon_.handleMessage(wparam, lparam, [=](UINT ti_msg, Point pt)
    {
      pt = getCursorPosition();

      switch (ti_msg)
      {
      case NIN_POPUPOPEN:
      case NIN_SELECT:
      case NIN_KEYSELECT:
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_CONTEXTMENU:
        onContextMenu(pt);
        return 1;
      }
      return 0;
    });

  // Messages des PopupMenus
  case WM_COMMAND: {
    int id    = LOWORD(wparam);
    int event = HIWORD(wparam);
    // Menüauswahl bearbeiten:
    switch (id)
    {
    case InfoEntry:
      openProjectWebsite();
      return 0;

    case InfoLicence:
      openLicence();
      return 0;

    case AutostartEntry:
      onAutostartSet(!popup_menu_.isEntryChecked(AutostartEntry));
      return 0;

    case QuitEntry:
      destroy();
      return 0;
    }
    return 0;
  }
  }

  // pass message to modules
  auto result = global_events_.handleWindowsMessage(msg, wparam, lparam);
  return result ? result.value() : Control::onMessage(msg, wparam, lparam);
}

void PowerWinApp::onContextMenu(Windows::Point pt)
{
  ::SetForegroundWindow(getNativeHandle());
  openPopupMenu(popup_menu_, pt, getNativeHandle());
}

void PowerWinApp::onAutostartSet(bool value)
{
  popup_menu_.check(AutostartEntry, value);
  setProgramToAutostart(value);
}

void PowerWinApp::openProjectWebsite()
{
  ::ShellExecuteW(getHWND(), L"open", POWERWIN_URL, nullptr, nullptr, SW_SHOW);
}

void PowerWinApp::openLicence()
{
  auto exe_dir = Windows::Path(Windows::Application::getExecutablePath())
                 .getFolder();
  auto licence_path = exe_dir + L"\\" POWERWIN_LICENCE_PATH;
  if (Windows::Path::exists(licence_path))
  {
    ::ShellExecuteW(
      getHWND(),
      L"open", licence_path.c_str(), nullptr, nullptr,
      SW_SHOW);
  }
  else
  {
    ::ShellExecuteW(
      getHWND(),
      L"open", POWERWIN_LICENCE_URL, nullptr, nullptr,
      SW_SHOW);
  }
}

} // namespace PowerWin

int APIENTRY wWinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  PWSTR pCmdLine,
  int nCmdShow)
{
  Windows::Application app(POWERWIN_PACKAGE_NAME, hInstance);
  return app.run(PowerWin::PowerWinApp::run);
}

///////////////////////////////////////////////////////////////////////////////
// Windowpicker

//auto windowpicker = MouseClickHook([](unsigned button, POINT pt) -> bool {
//  if (button == WM_LBUTTONDOWN) return true;
//  if (button != WM_LBUTTONUP) return false;
  
//  OutputDebugString(L"windowpicker\n");

//  HWND window = WindowFromPoint(pt);
//  if (window == NULL) {
//    OutputDebugString(L"Kein Fenster ausgewählt!\n");
//    return false;
//  } else {
//    wchar_t class_name[255];
//    GetClassNameW(window, class_name, sizeof(class_name));

//    OutputDebugString(L"Fensterklassenname: '");
//    OutputDebugString(class_name);
//    OutputDebugString(L"'\n");
//  }

//  return true;
//});

