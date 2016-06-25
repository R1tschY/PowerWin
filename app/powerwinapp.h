#pragma once

#include <vector>

#include <lightports/controls/window.h>
#include <lightports/extra/trayicon.h>
#include <windows.h>
#include "plugin.h"

class PowerWinApp : public Windows::Window {
  PowerWinApp();
  virtual ~PowerWinApp();

public:
  static int run();

  static PowerWinApp* get() {
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

  static PowerWinApp* instance_;

  static ATOM RegisterWinClass(HINSTANCE hInstance);
};

