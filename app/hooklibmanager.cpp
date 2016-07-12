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

#include <cpp-utils/strings/string_view.h>
#include <cpp-utils/strings/string_builder.h>
#include <cpp-utils/algorithm/container.h>
#include <lightports/extra/process.h>
#include <lightports/core/debug.h>
#include <lightports/base/application.h>

#include "messages.h"

using namespace Windows;

namespace PowerWin {

namespace  {

Process runDll(
    cpp::wstring_view rundll32_exe,
    cpp::wstring_view dll,
    cpp::wstring_view entry,
    cpp::wstring_view cmdln_args)
{
  std::wstring cmdln;
  cpp::concatenate_to(
        cmdln,
        L'"', rundll32_exe, L"\" ", dll, L',', entry, L' ', cmdln_args);

  print(L"%ls\n", cmdln.c_str());

  return Process::runCmdln(std::move(cmdln));
}

Process runDll32(cpp::wstring_view dll, cpp::wstring_view entry, cpp::wstring_view cmdln_args)
{
  return runDll(
        L"C:\\Windows\\System32\\rundll32.exe",
        dll, entry, cmdln_args
        );
}

Process runDll64(cpp::wstring_view dll, cpp::wstring_view entry, cpp::wstring_view cmdln_args)
{
  return runDll(
        L"C:\\Windows\\Sysnative\\rundll32.exe",
        dll, entry, cmdln_args
        );
}

} // namespace

HookLibManager::HookLibManager()
{

}

void HookLibManager::startLibs()
{
  runDll32(Path(Application::getExecutablePath()).getFolder() + L"\\libpowerwin32.dll", L"EnterGodModus", L"");
  if (Application::is64BitWindows())
  {
    runDll64(Path(Application::getExecutablePath()).getFolder() + L"\\libpowerwin64.dll", L"EnterGodModus", L"");
  }
}

void HookLibManager::unloadLibs()
{
  postMessage(WM_CLOSE);
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
  auto iter = std::find(hooklibs_.begin(), hooklibs_.end(), window);
  if (iter == hooklibs_.end())
  {
    hooklibs_.push_back(window);
  }
}

} // namespace PowerWin
