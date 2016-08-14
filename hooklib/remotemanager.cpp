/*
 * hookmanager.cpp
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#include <app/messages.h>
#include <cpp-utils/algorithm/container.h>
#include <cpp-utils/preprocessor.h>
#include <hooklib/remotemanager.h>
#include <lightports/base/application.h>
#include <lightports/core/debug.h>
#include <lightports/core/debugstream.h>
#include <minwindef.h>
#include <windef.h>
#include <winnt.h>
#include <winuser.h>
#include <exception>
#include <functional>
#include <iostream>

#include <modules/system-menu/systemmenuhook.h>

using namespace Windows;

namespace PowerWin {

RemoteManager::RemoteManager()
{
  create(CPP_TO_WIDESTRING(POWERWIN_APP_NAME) L"-Hooklib");
}

void RemoteManager::activate()
{
  DebugOutputStream() << __PRETTY_FUNCTION__ << ':' << __LINE__;

  if (activated_)
    return;
  activated_ = true;

  cpp::transform(
    HookModuleRegistry::entries(),
    modules_,
    [&](auto& entry){
      HookModuleContext context(entry.name());
      DebugOutputStream() << L"activated module " << entry.name();
      return entry.create(context);
  });
}

void RemoteManager::deactivate()
{
  DebugOutputStream() << __PRETTY_FUNCTION__ << ':' << __LINE__;
  if (!activated_)
    return;
  activated_ = false;

  modules_.clear();
}

void RemoteManager::onCreate()
{
  app_hwnd_ = Control::find(nullptr, L"PowerWinApp", nullptr);
  if (app_hwnd_ != nullptr)
  {
    PostMessage(
      app_hwnd_,
      Messages::RegisterHooklib,
      reinterpret_cast<WPARAM>(getNativeHandle()),
      0);
  }
  else
  {
    print(L"cannot find PowerWinApp window.");
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
void CALLBACK EnterGodModus(
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
    Windows::DebugOutputStream() << L"Unknown exception catched at " << CPP_TO_WIDESTRING(POWERWIN_APP_NAME) << std::endl;
  }
}

} // namespace PowerWin
