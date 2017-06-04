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

#include "wkill.h"

#include <windows.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QMouseEvent>

#include <app/configuration.h>
#include <app/hotkeymanager.h>
#include <app/log.h>
#include <app/hooklibmanager.h>
#include <lightports/os/process.h>
#include <lightports/user/cursor.h>

using namespace Windows;

namespace PowerWin {

WKillWindow::WKillWindow()
{
  auto* layout = new QHBoxLayout();
  setLayout(layout);

  activateButton_ = new QLabel(tr("Drag on Window"), this);
  layout->addWidget(activateButton_);

  resize(150, 300);
}

void WKillWindow::mousePressEvent(QMouseEvent* event)
{
  if (event->buttons() != Qt::LeftButton)
  {
    // exit through other mouse click
    choosing_ = false;
    setCursor(Qt::ArrowCursor);
    return;
  }

  setCursor(Qt::PointingHandCursor);
  choosing_ = true;
}

void WKillWindow::mouseReleaseEvent(QMouseEvent* event)
{
  if (!choosing_)
    return;

  hide();
  setCursor(Qt::ArrowCursor);
  choosing_ = false;

  try
  {
    // kill process of clicked window
    auto hwnd = Window::at(Point(event->globalX(), event->globalY()));
    if (!hwnd)
    {
      qCritical() << "wkill: no window at "
          << event->globalX() << "/" << event->globalY();
      return;
    }

    auto pid = hwnd.getProcessId();
    auto process = Process::open(Process::AccessRights::Terminate, pid);

    qInfo() << "wkill: terminate HWND:" << hwnd.getHWND() << "PID:" << pid;

    // TODO: do not kill own process, desktop process, ...
    //process.terminate(1);
  }
  catch(const Exception& exp)
  {
    qCritical() << "wkill: Terminate process failed:" << exp.what();
  }
}

WKill::WKill(ModuleContext& context)
: hotkey_(context.getHotkeyManager())
{
  hotkey_.setCallback([=](){ onHotkey(); });
  hotkey_.setKey(context
    .getConfiguration()
    .readValue(L"wkill", L"hotkey", L"Ctrl+Alt+X"));
}

void WKill::onHotkey()
{
  window_.show();
  window_.activateWindow();
  window_.raise();
}

ModuleRegistry::element<WKill> XWKill(
  L"wkill", L"kill process with click on window (xkill clone)"
);

} // namespace PowerWin
