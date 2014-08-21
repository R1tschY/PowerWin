#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>

#include "c++/algorithm.h"
#include "c++/array_ref.h"
#include "c++/boolean.h"
#include "DesktopHooks.h"
#include "windows/Hook.h"
#include "windows/ConfigFile.h"
#include "windows/utilwindow.h"
#include "windows/path.h"
#include "plugins/ActionsPlugin.h"
#include "plugins/ScrollPlugin.h"
#include "plugins/FullscreenPlugin.h"
#include "plugins/SystemMenuPlugin.h"
#include "windows/debug.h"
#include "macros.h"
#include "windows/application.h"
#include "windows/trayicon.h"

#ifdef ENV32BIT
# define POWERWIN_APP_NAME L"PowerWin32"
# define POWERWIN_64BIT_NAME L"PowerWin64"
#else
# define POWERWIN_APP_NAME L"PowerWin64"
#endif

namespace {

HINSTANCE dllinstance_ = nullptr;

// /////////////////////////////////////////////////////////////////////////////

class PowerWin {
public:
  PowerWin();

  static int run();

  static PowerWin* get() {
    return instance_;
  }

  HWND getWindow() {
    return window_;
  }

private:
  std::vector<std::unique_ptr<Plugin>> plugins_;

  Windows::TrayIcon tray_icon_;
  HWND window_;


  static PowerWin* instance_;

  static ATOM RegisterWinClass(HINSTANCE hInstance);
  void start(HWND hwnd);
  void onDestroy();

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

PowerWin* PowerWin::instance_ = nullptr;

PowerWin::PowerWin() :
  plugins_(),
  tray_icon_(),
  window_(nullptr)
{
#ifdef MAIN_MODULE
  plugins_.push_back(std::unique_ptr<Plugin>(new ActionsPlugin()));
  plugins_.push_back(std::unique_ptr<Plugin>(new ScrollPlugin()));
  //plugins_.push_back(std::unique_ptr<Plugin>(new FullscreenPlugin()));
  plugins_.push_back(std::unique_ptr<Plugin>(new SystemMenuPlugin()));
#else
  plugins_.push_back(std::unique_ptr<Plugin>(new SystemMenuPlugin()));
#endif
}

ATOM PowerWin::RegisterWinClass(HINSTANCE hInstance) {
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
  wcex.lpszClassName	= POWERWIN_APP_NAME;
  wcex.hIconSm		= 0;

  return RegisterClassEx(&wcex);
}

int PowerWin::run() {
  //  init Comctl32.dll
  /*const INITCOMMONCONTROLSEX icce = {
    sizeof(INITCOMMONCONTROLSEX),
    ICC_STANDARD_CLASSES
  };
  if (!InitCommonControlsEx(&icce)) {
    ERROR(L"%s\n", L"Kann 'common controls' nicht initailsieren!");
  }*/
  PowerWin powerwin;
  instance_ = &powerwin;

  RegisterWinClass(Windows::Application::getInstance());

  powerwin.window_ = CreateWindowW(
        POWERWIN_APP_NAME,
        POWERWIN_APP_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        0,
        CW_USEDEFAULT,
        0,
        NULL,
        NULL,
        Windows::Application::getInstance(),
        NULL);

#ifdef ENV32BIT
  print(L"%ls hwnd: %d\n", POWERWIN_APP_NAME, powerwin.window_);
#else
  print(L"%ls hwnd: %d\n", POWERWIN_APP_NAME, powerwin.window_);
#endif

  // main message loop
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  print(L"%ls: The end\n", POWERWIN_APP_NAME);

  return 0;
}

bool RunCmdln(std::wstring cmdln) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  cpp::zero(si);
  cpp::zero(pi);

  si.cb = sizeof(si);
  si.dwFlags |= STARTF_USESTDHANDLES;
  si.hStdInput = nullptr;
  si.hStdError = nullptr;
  si.hStdOutput = nullptr;

  BOOL success = CreateProcessW(nullptr, &(*cmdln.begin()), nullptr,
                                nullptr, false, 0, nullptr, nullptr, &si, &pi);
  if (success) {
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
  return success;
}

bool StartProgram(cpp::wstring_ref exe_path, std::wstring args) {
  std::wstring cmdln;
  cmdln += '\"';
  cmdln += exe_path;
  cmdln += '\"';
  cmdln += ' ';
  cmdln += args;

  return RunCmdln(cmdln);
}

bool RunDll64Bit(cpp::wstring_ref dll_name, cpp::wstring_ref entry, cpp::wstring_ref cmdln_args)
{
  const wchar_t* rundll32_exe = L"C:\\Windows\\Sysnative\\rundll32.exe";

  std::wstring cmdln;
  cmdln += rundll32_exe;
  cmdln += L' ';
  cmdln += dll_name;
  cmdln += L',';
  cmdln += entry;
  cmdln += L' ';
  cmdln += cmdln_args;

  print(L"%ls\n", cmdln.c_str());

  if (!Windows::PathExists(dll_name)) {
    print(L"RunDll64Bit: '%ls' does not exist.\n", dll_name.begin());
    return false;
  }
  
  return RunCmdln(std::move(cmdln));
}

void PowerWin::start(HWND hwnd) {
  window_ = hwnd;

  print(L"PowerWin::start\n");

  ConfigFile config;
  config.loadFromFile(Windows::Application::getExecutablePath() + L"\\config.ini");

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


  /*tray_icon.add(WinExtra::getMainWindow(),
                ExtractIcon(
                  Windows::Application::getInstance(),
                  L"C:\\Windows\\system32\\shell32.dll",
                  -26));*/
#ifdef MAIN_MODULE
  tray_icon_.add(window_, LoadIcon(NULL, IDI_APPLICATION));

  // start 64Bit-DLL
  if (Windows::Application::Is64BitWindows()) {
    print(L"Start 64-bit process\n");
    RunDll64Bit(Windows::Application::getExecutablePath() + L"\\libpowerwin64.dll", L"KeepTheCarRunning", L"");
  }
#endif
}

void PowerWin::onDestroy() {
#ifdef MAIN_MODULE
  // exit 64Bit-DLL
  if (Windows::Application::Is64BitWindows()) {
    SetLastError(0);
    HWND window_64bit = FindWindowW(POWERWIN_64BIT_NAME, nullptr);
    if (window_64bit == nullptr) {
      print(L"Cannot find window to 64Bit-Process: %ls\n", Windows::GetLastWindowsError().c_str());
    } else {
      print(L"Exit 64-bit process. window: %d\n", window_64bit);
      if (!PostMessage(window_64bit, WM_CLOSE, 0, 0)) {
        print(L"PostMessage to 64-bit process window failed: %ls\n", Windows::GetLastWindowsError().c_str());
      }
    }
  }
#endif

  // deactivare all plugins
  try {
    for (auto&& plugin : plugins_) {
      print(L"Deactivate plugin %ls\n", plugin->getName());
      plugin->deactivate();
    }
  } catch (...) {
    MessageBox(NULL, L"Ups", L"!!!", MB_ICONERROR | MB_OK);
  }

  // remove tray icon
  tray_icon_.remove();

  // exit process
  PostQuitMessage(0);
}

LRESULT CALLBACK PowerWin::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg) {
  case WM_CREATE:
    PowerWin::get()->start(hwnd);
    break;

  case WM_DESTROY:
    PowerWin::get()->onDestroy();
    break;

  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;

  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
  return 0;
}

} // namespace

extern "C" {

HINSTANCE win_getDllInstance() {
  if (dllinstance_ == 0) {
    print(L"win_getDllInstance: too early access of dll instance!\n");
  }

  return dllinstance_;
}

void win_updateDllInstance(HINSTANCE instance) {
  if (dllinstance_ == NULL) {
    dllinstance_ = instance;
  }
}

HWND win_getMainWindow() {
  PowerWin* instance = PowerWin::get();
  if (instance == nullptr) {
    print(L"not in powerwin process!\n");
  }

  HWND window = instance->getWindow();
  if (window == nullptr) {
    print(L"too early access of main window!\n");
  }

  return window;
}

void CALLBACK KeepTheCarRunning(HINSTANCE hInstance,
                                HINSTANCE hPrevInstance,
                                LPSTR lpCmdLine,
                                int nCmdShow)
{
  Windows::Application app(POWERWIN_APP_NAME, hInstance);
  app.run(PowerWin::run);
}

void win_destroy() {
  DestroyWindow(win_getMainWindow());
}

} // extern "C"

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

