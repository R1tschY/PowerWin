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

#include "hooklibmanager.h"

#include <QString>
#include <QProcess>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>

#include <cpp-utils/algorithm/container.h>
#include <lightports/user/application.h>

#include "messages.h"

namespace PowerWin {

void HookLibManager::runDll(
    int bitness,
    const QString& dll,
    const QString& entry)
{
  if (!QFileInfo::exists(dll))
  {
    qWarning() << "needed hook dll" << dll << "does not exist (same features will not work).";
    return;
  }

  QStringList arguments;
  arguments << (dll + ',' + entry);

  QString rundll32_exe =
      bitness == 32
      ? "C:/Windows/System32/rundll32.exe"
      : "C:/Windows/Sysnative/rundll32.exe";

  processes_.emplace_back(new QProcess());
  processes_.back()->start(rundll32_exe, arguments);
}

HookLibManager::HookLibManager() = default;
HookLibManager::~HookLibManager() = default;

void HookLibManager::startLibs()
{
  runDll(
      32,
      QCoreApplication::applicationDirPath() + "/libpowerwin32.dll",
      "PatchWindows@16"
  );
  if (Windows::Application::is64BitWindows())
  {
    runDll(
        64,
        QCoreApplication::applicationDirPath() + "/libpowerwin64.dll",
        "PatchWindows"
    );
  }
}

void HookLibManager::unloadLibs()
{
  postMessage(WM_CLOSE);

  for (auto& process : processes_)
  {
    process->terminate();
  }

  for (auto& process : processes_)
  {
    process->waitForFinished(1000);
  }
}

void HookLibManager::sendMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  cpp::for_each(hooklibs_, [=](HWND hwnd){
    ::SendMessageW(hwnd, msg, wparam, lparam);
  });
}

void HookLibManager::postMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  cpp::for_each(hooklibs_, [=](HWND hwnd){
    ::PostMessageW(hwnd, msg, wparam, lparam);
  });
}

void HookLibManager::registerHookLib(HWND window)
{
  auto iter = cpp::find(hooklibs_, window);
  if (iter == hooklibs_.end())
  {
    hooklibs_.push_back(window);
  }
}

} // namespace PowerWin
