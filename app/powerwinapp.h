///
/// Copyright (c) 2016 R1tschY
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#pragma once

#include <vector>

#include <lightports/controls/messagesink.h>
#include <lightports/shell/trayicon.h>
#include <lightports/user/menu.h>
#include <windows.h>

#include "module.h"
#include "modulemanager.h"
#include "hotkeymanager.h"
#include "configuration.h"
#include "hooklibmanager.h"
#include "globalevents.h"

namespace PowerWin {

class PowerWinApp : public Windows::MessageSink {
  PowerWinApp();
  ~PowerWinApp();

public:
  static int run();

  HWND getWindow() {
    return getHWND();
  }

private:
  enum
  {
    InfoMenu,
    InfoEntry,
    InfoLicence,

    AutostartEntry,
    QuitEntry
  };

  Windows::TrayIcon tray_icon_;
  Windows::Menu popup_menu_;
  Windows::Menu info_menu_;

  Configuration configuration_;
  HotkeyManager hotkeys_;
  GlobalEvents global_events_;

  ModuleManager modules_;
  HookLibManager hooklibs_;

  Hotkey quit_shortcut_;

  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;
  void onCreate() override;
  void onDestroy() override;

  void onContextMenu(Windows::Point pt);
  void onAutostartSet(bool value);

  void openProjectWebsite();
  void openLicence();
};

} // namespace PowerWin

