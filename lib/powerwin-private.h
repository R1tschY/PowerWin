#pragma once

#include <vector>

#include <windows/controls/window.h>
#include <windows/trayicon.h>
#include "Plugin.h"
#include <windows.h>

class PowerWin : public Windows::Window {
  PowerWin();
  virtual ~PowerWin();

public:
  static int run();

  static PowerWin* get() {
    return instance_;
  }

  HWND getWindow() {
    return getNativeHandle();
  }

  void onCreate() override;
  void onDestroy() override;

private:
  std::vector<std::unique_ptr<Plugin>> plugins_;

  Windows::TrayIcon tray_icon_;

  static PowerWin* instance_;

  static ATOM RegisterWinClass(HINSTANCE hInstance);
};

