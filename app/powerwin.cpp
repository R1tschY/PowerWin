#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <iostream>

#include "c++/algorithm.h"
#include "c++/arrayref.h"
#include "c++/boolean.h"
#include "c++/stringliteral.h"
#include <windows/core.h>
#include <windows/controls/utilwindow.h>
#include <windows/base/path.h>
#include <windows/base/application.h>
#include <windows/extra/trayicon.h>
#include <windows/controls.h>
#include <windows/base/configfile.h>
#include <windows/extra/rundll.h>
#include <windows/controls/gdipluscontext.h>

#include "powerwin.h"

#include <app/powerwin.h>
#include "plugins/ActionsPlugin.h"
#include "plugins/ScrollPlugin.h"
#include "plugins/FullscreenPlugin.h"
#include "plugins/SystemMenuPlugin.h"
#include "plugins/SplashScreenPlugin.h"
#include "plugins/quickstarter.h"
#include "../hooklib/macros.h"

#include "../hooklib/remotemanager.h"
#include <thread>

PowerWin* PowerWin::instance_ = nullptr;

PowerWin::PowerWin() :
  Window(Window::Type::Normal),
  plugins_(),
  tray_icon_()
{
  plugins_.emplace_back(new ActionsPlugin());
  plugins_.emplace_back(new ScrollPlugin());
  //plugins_.emplace_back(new FullscreenPlugin());
  plugins_.emplace_back(new SystemMenuPlugin());
  plugins_.emplace_back(new SplashScreenPlugin());
  plugins_.emplace_back(new QuickStarterPlugin());
}

PowerWin::~PowerWin() {  }

int PowerWin::run() {
  //  init Comctl32.dll
  /*const INITCOMMONCONTROLSEX icce = {
    sizeof(INITCOMMONCONTROLSEX),
    ICC_STANDARD_CLASSES
  };
  if (!InitCommonControlsEx(&icce)) {
    ERROR(L"%s\n", L"Kann 'common controls' nicht initailsieren!");
  }*/

  // start hook libs

  std::thread hook_thread([](){ EnterGodModus(nullptr, nullptr, nullptr, 0); });

  // start 64Bit-DLL
  if (Windows::Application::Is64BitWindows()) {
    print(L"Start 64-bit process\n");
    Windows::RunDll::execute64BitDll(Windows::Application::getExecutablePath().getFolder() + L"\\libpowerwin64.dll", L"EnterGodModus", L"");
    // TODO: error logging
  }

  Sleep(100);

  Windows::GdiplusContext gdi;

  PowerWin powerwin;
  instance_ = &powerwin;

  powerwin.create(nullptr, wstring_literal(POWERWIN_APP_NAME));

  print(L"%ls hwnd: %d\n", POWERWIN_APP_NAME_W, powerwin.getNativeHandle());

  Windows::Application::processMessages();

  hook_thread.join();

  print(L"%ls: The end\n", POWERWIN_APP_NAME_W);

  return 0;
}

void PowerWin::onCreate() {
  print(L"PowerWin::start\n");

  Windows::ConfigFile config;
  config.loadFromFile(Windows::Application::getExecutablePath().toString()
		  + L"\\config.ini");

  for (auto&& plugin : plugins_) {
    Plugin::Options opts;
    for (const std::wstring& key : config.getKeys(plugin->getName())) {
      opts[key] = config.getString(plugin->getName(), key.c_str(), nullptr);
    }
    plugin->setOptions(std::move(opts));

    if (plugin->getBooleanOption(L"active", true)) {
      print(L"activate plugin (0x%p) %ls\n", plugin.get(), plugin->getName());
      plugin->activate();
    }
  }

//  tray_icon_.add(getWindow(), LoadIcon(NULL, IDI_APPLICATION), wstring_literal(POWERWIN_APP_NAME));
  tray_icon_.add(getNativeHandle(),
                ExtractIcon(
                  Windows::Application::getInstance(),
                  L"C:\\Windows\\system32\\shell32.dll",
                  -26),
                 wstring_literal(POWERWIN_APP_NAME));
}

void PowerWin::onDestroy() {
  // exit 64Bit-DLL
  if (Windows::Application::Is64BitWindows()) {
    SetLastError(0);
    HWND window_64bit = FindWindowW(POWERWIN_64BIT_NAME_W, nullptr);
    if (window_64bit == nullptr) {
      // TODO check GetLastError() maybe the window is really not there
      print(L"Cannot find window to 64Bit-Process: %ls\n", Windows::GetLastWindowsError().c_str());
    } else {
      print(L"Exit 64-bit process. window: %d\n", window_64bit);
      if (!PostMessage(window_64bit, WM_CLOSE, 0, 0)) {
        print(L"PostMessage to 64-bit process window failed: %ls\n", Windows::GetLastWindowsError().c_str());
      }
    }
  }

  // deactivate all plugins
  try {
    for (auto&& plugin : plugins_) {
      print(L"Deactivate plugin %ls\n", plugin->getName());
      plugin->deactivate();
    }
  } catch (...) {
    MessageBoxW(NULL, L"Ups", L"!!!", MB_ICONERROR | MB_OK);
  }

  // remove tray icon
  tray_icon_.remove();

  // exit process
  PostQuitMessage(0);
}

extern "C"
void CALLBACK FixWindows(HINSTANCE hInstance,
                         HINSTANCE hPrevInstance,
                         LPSTR lpCmdLine,
                         int nCmdShow)
{

}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
  Windows::Application app(wstring_literal(POWERWIN_APP_NAME), hInstance);
  return app.run(PowerWin::run);
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

