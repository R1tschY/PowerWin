#pragma once

#include <vector>

#include <lightports/controls/window.h>
#include <lightports/extra/trayicon.h>
#include <windows.h>

#include "module.h"
#include "modulemanager.h"
#include "hotkeymanager.h"
#include "configuration.h"
#include "hooklibmanager.h"

namespace PowerWin {

/* TODO
 * - Action backend: powerwin.exit(), windows.start("cmd.exe"), ...
 *   class ActionManager { registerFunction(module, funcname, func); };
 * - LuaBrigde
 * - Log-Backend and log-frondend from module
 */

class PowerWinApp : public Windows::Window {
  PowerWinApp();
  ~PowerWinApp();

public:
  static int run();

  HWND getWindow() {
    return getNativeHandle();
  }

  void onCreate() override;
  void onDestroy() override;

private:
  Windows::TrayIcon tray_icon_;

  Configuration configuration_;
  ModuleManager modules_;
  HotkeyManager hotkeys_;
  HookLibManager hooklibs_;

  static ATOM RegisterWinClass(HINSTANCE hInstance);

  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;
};

} // namespace PowerWin

