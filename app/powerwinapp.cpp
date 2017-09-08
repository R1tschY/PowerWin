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

#include "powerwinapp.h"

#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <iostream>

#include <cpp-utils/preprocessor.h>
#include <cpp-utils/strings/string_literal.h>
#include <lightports/core.h>
#include <lightports/controls/utilwindow.h>
#include <lightports/os/path.h>
#include <lightports/user/application.h>
#include <lightports/user/resources.h>
#include <lightports/core/charcodecs.h>
#include <lightports/shell/trayicon.h>
#include <lightports/controls.h>
#include <lightports/extra/configfile.h>
#include <lightports/controls/gdipluscontext.h>
#include <lightports/user/systeminformation.h>
#include <lightports/user/cursor.h>
#include <lightports/extra/autostart.h>

#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QEvent>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QFile>

#include "messages.h"

#include "../hooklib/macros.h"

#include "../hooklib/remotemanager.h"
#include <powerwin-config.h>

#ifdef QT_STATIC
# include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

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
  QSystemTrayIcon(),
  MessageSink(registerWindowClass()),
  hotkeys_(),
  global_events_(),
  hooklibs_(),
  modules_(configuration_, hotkeys_, global_events_, hooklibs_),
  quit_shortcut_(hotkeys_)
{
  configuration_.loadIniFile(
      QString(
        QCoreApplication::applicationDirPath()
        + QLatin1String("/config.ini")
      ).toStdWString()
  );

  //
  autostart_action_ = new QAction(tr("Start with Windows"), this);
  autostart_action_->setCheckable(true);
  autostart_action_->setChecked(isProgramInAutostart());

  // context menu
  createContextMenu();

  // tray icon
  setIcon(QIcon(QStringLiteral(":/powerwin/img/powerwin16.png")));
  setToolTip(POWERWIN_PACKAGE_NAME_ASCII);
  connect(
      this, &QSystemTrayIcon::activated,
      this, &PowerWinApp::onTrayIconActivated);

  // hotkeys
  auto quit_shortcut = configuration_.readValue(L"powerwin", L"quit", L"Ctrl+F12");
  quit_shortcut_.setCallback([=](){ qApp->quit(); });
  quit_shortcut_.setKey(quit_shortcut);

  // local modules
  modules_.loadModules();

  // hook modules
  MessageSink::create(L"PowerWinApp");
  hooklibs_.startLibs();
}

PowerWinApp::~PowerWinApp()
{
  qInfo() << L"PowerWinApp::onDestroy\n";

  hide();

  hooklibs_.unloadLibs();

  // deactivate all plugins
  modules_.unloadModules();
}

LRESULT PowerWinApp::onMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
  // internal management messages
  case Messages::RegisterHooklib:
    hooklibs_.registerHookLib(reinterpret_cast<HWND>(wparam));
    return 0;

  // internal messages
  case WM_NCCREATE:
  case WM_CREATE:
  case WM_DESTROY:
  case WM_NCDESTROY:
    return Control::onMessage(msg, wparam, lparam);

  // filter meaningless messages
  case WM_MOVE:
  case WM_SIZE:
    return ::DefWindowProc(getHWND(), msg, wparam, lparam);
  }

  // pass message to modules
  // TODO: remove
  auto result = global_events_.handleWindowsMessage(msg, wparam, lparam);
  return result ? result.value() : Control::onMessage(msg, wparam, lparam);
}

void PowerWinApp::createContextMenu()
{
  popup_menu_ = std::make_unique<QMenu>();

  auto* info_menu = popup_menu_->addMenu(tr("Info"));
  info_menu->addAction(
      QString::fromWCharArray(POWERWIN_PACKAGE_NAME L" " POWERWIN_PACKAGE_VERSION),
      this, &PowerWinApp::openProjectWebsite);
  info_menu->addAction(tr("Copyright © 2017 R1tschY"),
      this, &PowerWinApp::openProjectWebsite);
  info_menu->addAction(tr("Licenced under GPL v3"),
      this, &PowerWinApp::openLicence);

  popup_menu_->addSeparator();
  popup_menu_->addAction(autostart_action_);
  popup_menu_->addSeparator();
  popup_menu_->addAction(tr("Quit"), qApp, &QCoreApplication::quit);
}

void PowerWinApp::onAutostartSet()
{
  setProgramToAutostart(autostart_action_->isChecked());
}

void PowerWinApp::openProjectWebsite()
{
  QDesktopServices::openUrl(QUrl(QString::fromWCharArray(POWERWIN_URL)));
}

void PowerWinApp::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch(reason)
  {
  case QSystemTrayIcon::DoubleClick:
  case QSystemTrayIcon::Trigger:
  case QSystemTrayIcon::Context:
    popup_menu_->exec(QCursor::pos());
    break;
  default:;
  }
}

void PowerWinApp::openLicence()
{
  auto licence_path = QCoreApplication::applicationDirPath()
    + QString::fromWCharArray(L"/" POWERWIN_LICENCE_PATH);
  if (QFileInfo(licence_path).exists())
  {
    QDesktopServices::openUrl(QUrl("file:///" + licence_path));
  }
  else
  {
    QDesktopServices::openUrl(QUrl(QString::fromWCharArray(POWERWIN_LICENCE_URL)));
  }
}

} // namespace PowerWin

int main(int argc, char *argv[])
{
  using namespace PowerWin;

  //SharedLock lock("PowerWin");
  //if (!lock.hasLock())
  //  return 1;

  Windows::Application app(L"", ::GetModuleHandle(nullptr));
  QApplication qtapp(argc, argv);
  qtapp.setQuitOnLastWindowClosed(false);

  QTranslator translator;
  if (translator.load(
    QLocale(),
    QLatin1String("PowerWinApp"),
    QLatin1String("_"),
    QLatin1String(":/translations")))
  {
    qtapp.installTranslator(&translator);
  }
  else
  {
    qCritical() << "Translations for PowerWin cannot be loaded.";
  }

  PowerWinApp powerwin;
  powerwin.show();

  return qtapp.exec();
}

