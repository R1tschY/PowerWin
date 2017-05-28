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

#include <app/messages.h>
#include <cpp-utils/algorithm/container.h>
#include <cpp-utils/preprocessor.h>
#include <hooklib/remotemanager.h>
#include <lightports/user/application.h>
#include <lightports/core/debug.h>
#include <lightports/core/debugstream.h>
#include <windows.h>
#include <exception>
#include <functional>

using namespace Windows;

namespace PowerWin {

RemoteManager::RemoteManager()
{
  create(CPP_TO_WIDESTRING(POWERWIN_APP_NAME) L"-Hooklib");
}

void RemoteManager::activate()
{
  if (activated_)
    return;
  activated_ = true;

  cpp::transform(
    HookModuleRegistry::entries(),
    modules_,
    [&](const HookModuleRegistry::entry& entry) {
      HookModuleContext context(entry.name(), app_hwnd_.getHWND());
      // TODO: catch errors
      DebugOutputStream() << L"activated module " << entry.name();
      return entry.create(context);
  });
}

void RemoteManager::deactivate()
{
  if (!activated_)
    return;
  activated_ = false;

  modules_.clear();
}

void RemoteManager::onCreate()
{
  app_hwnd_ = Window::find(L"PowerWinApp", nullptr);
  if (app_hwnd_ != nullptr)
  {
    PostMessage(
      app_hwnd_.getHWND(),
      Messages::RegisterHooklib,
      reinterpret_cast<WPARAM>(getHWND()),
      0);
  }
  else
  {
    ::OutputDebugStringW(L"cannot find PowerWinApp window.");
  }

  activate();
}

void RemoteManager::onDestroy()
{
  // end hook lib if window is destroyed
  PostQuitMessage(0);
}

LRESULT RemoteManager::onMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  cpp::optional<LRESULT> result;
  for (auto& module : modules_)
  {
    result = module->processMessage(msg, wparam, lparam);
    if (result)
      return result.value();
  }

  switch (msg)
  {
  case WM_CLOSE:
    // destroy window on close request
    destroy();
    return 0;

  default:;
  }

  return Windows::Control::onMessage(msg, wparam, lparam);
}

extern "C"
void CALLBACK PatchWindows(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
  try
  {
    Windows::Application app(CPP_TO_WIDESTRING(POWERWIN_APP_NAME) L"-Hooklib", hInstance);
    app.run([](){
      RemoteManager mgr;
      return Windows::Application::processMessages();
    });
  }
  catch (const std::exception& e)
  {
    Windows::DebugOutputStream() << L"Exception catched at " << CPP_TO_WIDESTRING(POWERWIN_APP_NAME) << L": " << e.what() << std::endl;
  }
  catch (...)
  {
    ::OutputDebugStringW(L"Unknown exception catched at " CPP_TO_WIDESTRING(POWERWIN_APP_NAME));
  }
}

} // namespace PowerWin
