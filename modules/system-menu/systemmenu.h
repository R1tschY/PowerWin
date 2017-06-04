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

#ifndef PLUGINS_SPLASHSCREENPLUGIN_H_
#define PLUGINS_SPLASHSCREENPLUGIN_H_

#include <windows.h>

#include <QObject>

#include <app/module.h>
#include <app/helper/observermixin.h>
#include <app/globalevents.h>


class SystemMenuModule :
    private QObject,
    private PowerWin::ObserverMixin,
    public PowerWin::Module
{
  Q_OBJECT
public:
  SystemMenuModule(PowerWin::ModuleContext& context);
  ~SystemMenuModule();

private:
  UINT new_window_msg_;
  UINT toggle_topmost_msg_;

  void onGlobalMessage(PowerWin::WindowsMessageEvent& e);
  void onNewWindow(HWND hwnd);
  void onToogleTopmost(HWND hwnd);
};

#endif /* PLUGINS_SPLASHSCREENPLUGIN_H_ */
