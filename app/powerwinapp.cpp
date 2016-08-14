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
#include <lightports/controls/gdipluscontext.h>
#include <lightports/extra/systeminformation.h>

#include "resources.h"
#include "messages.h"

#include "../hooklib/macros.h"

#include "../hooklib/remotemanager.h"
#include <thread>

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
  modules_(configuration_, hotkeys_),
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

  powerwin.create(wstring_literal(POWERWIN_APP_NAME));

  print(L"%ls hwnd: %d\n", CPP_TO_WIDESTRING(POWERWIN_APP_NAME), powerwin.getNativeHandle());

  Windows::Application::processMessages();

  print(L"%ls: The end\n", CPP_TO_WIDESTRING(POWERWIN_APP_NAME));

  return 0;
}

void PowerWinApp::onCreate() {
  print(L"PowerWin::start\n");
  DebugOutputStream() << L"ClassName: " << getClassName(getNativeHandle());

  configuration_.loadIniFile(
    Application::getExecutablePath() + L"\\config.ini"
  );

  // tray icon
  tray_icon_.setIcon(POWERWIN_ICON_SMALL);
  tray_icon_.setToolTip(wstring_literal(POWERWIN_APP_NAME));
  tray_icon_.add(getNativeHandle());

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
  Windows::DebugOutputStream() << L"PowerWinApp::onDestroy\n";

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
  case Messages::RegisterHooklib:
    hooklibs_.registerHookLib(reinterpret_cast<HWND>(wparam));
    return 0;

  }

  return Control::onMessage(msg, wparam, lparam);
}

} // namespace PowerWin

int APIENTRY wWinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  PWSTR pCmdLine,
  int nCmdShow)
{
  Windows::Application app(wstring_literal(POWERWIN_APP_NAME), hInstance);
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

