#include <string>
#include <algorithm>

#include "algorithm.h"
#include "DesktopHooks.h"
#include "windows/Hook.h"
#include "windows/ConfigFile.h"
#include "windows/utilwindow.h"
#include "plugins/ActionsPlugin.h"
#include "plugins/ScrollPlugin.h"
#include "plugins/FullscreenPlugin.h"
#include "plugins/SystemMenuPlugin.h"
#include "windows/debug.h"
#include "macros.h"
#include "PluginManager.h"
#include "windows/application.h"
#include "windows/trayicon.h"

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

static HINSTANCE dllinstance_ = nullptr;
static HWND window = 0;

SharedMemory* shared_memory;

static struct {
  ActionsPlugin actions;
  ScrollPlugin scroll;
//  FullscreenPlugin fullscreen;
  SystemMenuPlugin systemmenu;
} plugins;

static Windows::TrayIcon tray_icon;

ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style		= CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	= WndProc;
  wcex.cbClsExtra	= 0;
  wcex.cbWndExtra	= 0;
  wcex.hInstance	= hInstance;
  wcex.hIcon		= 0;
  wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName	= 0;
  wcex.lpszClassName	= L"PowerWin";
  wcex.hIconSm		= 0;

  return RegisterClassEx(&wcex);
}


static
int app_entry() {
  //  init Comctl32.dll
  /*const INITCOMMONCONTROLSEX icce = {
    sizeof(INITCOMMONCONTROLSEX),
    ICC_STANDARD_CLASSES | ICC_LINK_CLASS | ICC_UPDOWN_CLASS
  };
  if (!InitCommonControlsEx(&icce)) {
    ERROR(L"%s\n", L"Kann 'common controls' nicht initailsieren!");
  }*/

  print(L"app_entry %d\n", GetCurrentThreadId());

  MyRegisterClass(Windows::Application::getInstance());

  window = CreateWindowW(L"PowerWin",
                         L"PowerWin",
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT,
                         0,
                         CW_USEDEFAULT,
                         0,
                         NULL,
                         NULL,
                         Windows::Application::getInstance(),
                         NULL);

  // main message loop
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  print(L"the end\n");

  return 0;
}

namespace WinExtra {

HINSTANCE getDllInstance() {
  if (dllinstance_ == 0) {
    print(L"too early access of dll instance!");
  }

  return dllinstance_;
}

void updateDllInstance(HINSTANCE instance) {
  if (dllinstance_ == NULL) {
    dllinstance_ = instance;
  }
}

HWND getMainWindow() {
  if (window == 0) {
    print(L"too early access of main window!");
  }

  return window;
}

int run(HINSTANCE hInstance) {
  Windows::Application app(L"PowerWin", hInstance);
  return app.run(app_entry);
}

void destroy() {
  DestroyWindow(window);
}

} //  namespace WinExtra

static
void start() {
  ConfigFile config;
  config.loadFromFile(Windows::Application::getExeDir());

  PluginManager::get().forEach([&](Plugin* plugin) {
    Plugin::Options opts;
    for (const std::wstring& key : config.getKeys(plugin->getName())) {
      opts[key] = config.getString(plugin->getName(), key.c_str(), nullptr);
    }
    plugin->setOptions(std::move(opts));

    if (plugin->getBooleanOption(L"active", true)) {
      print(L"activate plugin (0x%p) %s\n", plugin, plugin->getName());
      plugin->activate();
    }
  });

  /*tray_icon.add(WinExtra::getMainWindow(),
                ExtractIcon(
                  Windows::Application::getInstance(),
                  L"C:\\Windows\\system32\\shell32.dll",
                  -26));*/

  tray_icon.add(window, LoadIcon(NULL, IDI_APPLICATION));
}

void onDestroy() {
  try {
    PluginManager::get().forEach([&](Plugin* plugin) {
      print(L"deactivate plugin %s\n", plugin->getName());
      plugin->deactivate();
    });
  } catch (...) {
    MessageBox(NULL, L"Ups", L"!!!", MB_ICONERROR | MB_OK);
  }

  tray_icon.remove();

  PostQuitMessage(0);
}

static
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_CREATE:
    window = hwnd;
    start();
    break;

  case WM_DESTROY:
    onDestroy();
    break;

  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
  return 0;
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

