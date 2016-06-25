#include "powerwinapp.h"

#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <iostream>

#include <cpp-utils/strings/string_literal.h>
#include <lightports/core.h>
#include <lightports/controls/utilwindow.h>
#include <lightports/base/path.h>
#include <lightports/base/application.h>
#include <lightports/base/resources.h>
#include <lightports/extra/trayicon.h>
#include <lightports/controls.h>
#include <lightports/base/configfile.h>
#include <lightports/extra/rundll.h>
#include <lightports/controls/gdipluscontext.h>

#include "resources.h"

#include "plugins/ActionsPlugin.h"
#include "plugins/ScrollPlugin.h"
#include "plugins/FullscreenPlugin.h"
#include "plugins/SystemMenuPlugin.h"
#include "plugins/SplashScreenPlugin.h"
#include "../hooklib/macros.h"

#include "../hooklib/remotemanager.h"
#include <thread>

PowerWinApp* PowerWinApp::instance_ = nullptr;

PowerWinApp::PowerWinApp() :
  Window(Window::Type::Normal),
  plugins_(),
  tray_icon_()
{
  plugins_.emplace_back(new ActionsPlugin());
  plugins_.emplace_back(new ScrollPlugin());
  plugins_.emplace_back(new FullscreenPlugin());
  plugins_.emplace_back(new SystemMenuPlugin());
  plugins_.emplace_back(new SplashScreenPlugin());
}

PowerWinApp::~PowerWinApp() {  }

int PowerWinApp::run() {
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
    Windows::RunDll::execute64BitDll(Windows::Path(Windows::Application::getExecutablePath()).getFolder() + L"\\libpowerwin64.dll", L"EnterGodModus", L"");
    // TODO: error logging
  }

  Sleep(100);

  Windows::GdiplusContext gdi;

  PowerWinApp powerwin;
  instance_ = &powerwin;

  powerwin.create(nullptr, wstring_literal(POWERWIN_APP_NAME));

  print(L"%ls hwnd: %d\n", CPP_TO_WIDESTRING(POWERWIN_APP_NAME), powerwin.getNativeHandle());

  Windows::Application::processMessages();

  hook_thread.join();

  print(L"%ls: The end\n", CPP_TO_WIDESTRING(POWERWIN_APP_NAME));

  return 0;
}

void PowerWinApp::onCreate() {
  print(L"PowerWin::start\n");

  Windows::ConfigFile config;
  config.loadFromFile(Windows::Application::getExecutablePath()
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
    Windows::Resources::getIcon(GetModuleHandle(nullptr), POWERWIN_ICON_SMALL),
                 wstring_literal(POWERWIN_APP_NAME));
}

void PowerWinApp::onDestroy() {
  // exit 64Bit-DLL
  if (Windows::Application::Is64BitWindows()) {
    SetLastError(0);
    HWND window_64bit = FindWindowW(CPP_TO_WIDESTRING(POWERWIN_64BIT_NAME), nullptr);
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

int APIENTRY wWinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  PWSTR pCmdLine,
  int nCmdShow)
{
  Windows::Application app(wstring_literal(POWERWIN_APP_NAME), hInstance);
  return app.run(PowerWinApp::run);
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

