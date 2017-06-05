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

#include <memory>

#include <lightports/controls/messagesink.h>
#include <lightports/shell/trayicon.h>
#include <lightports/user/menu.h>
#include <windows.h>

#include <QObject>
#include <QSystemTrayIcon>

#include "module.h"
#include "modulemanager.h"
#include "hotkeymanager.h"
#include "configuration.h"
#include "hooklibmanager.h"
#include "globalevents.h"

class QSystemTrayIcon;
class QMenu;
class QAction;
class QEvent;

namespace PowerWin {

class PowerWinApp : public QSystemTrayIcon, private Windows::MessageSink
{
public:
  PowerWinApp();
  ~PowerWinApp();

  // TODO: remove
  HWND getWindow() {
    return getHWND();
  }

  using QSystemTrayIcon::show;
  using QSystemTrayIcon::hide;

private:
  Configuration configuration_;
  HotkeyManager hotkeys_;
  GlobalEvents global_events_;

  HookLibManager hooklibs_;
  ModuleManager modules_;

  Hotkey quit_shortcut_;

  QAction* autostart_action_ = nullptr;
  std::unique_ptr<QMenu> popup_menu_ = nullptr;

  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;

  void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

  void createContextMenu();

  void onAutostartSet();

  void openProjectWebsite();
  void openLicence();
};

} // namespace PowerWin

