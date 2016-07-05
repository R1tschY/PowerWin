/*
 * hookmanager.cpp
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#include "remotemanager.h"

#include <functional>
#include <thread>
#include <string>
#include <cpp-utils/preprocessor.h>
#include <cpp-utils/algorithm/container.h>
#include "macros.h"
#include <lightports/core/debug.h>
#include <lightports/base/application.h>
#include <app/messages.h>

namespace PowerWin {

RemoteManager::RemoteManager()
{
  create(CPP_TO_WIDESTRING(POWERWIN_APP_NAME) L"-Hooklib");

  cpp::transform(
    HookModuleRegistry::entries(),
    modules_, [](auto& entry){ return entry.create(); });
}

void RemoteManager::activate()
{
  cpp::for_each(modules_, std::mem_fn(&HookModule::activate));
}

void RemoteManager::deactivate()
{
  cpp::for_each(modules_, std::mem_fn(&HookModule::deactivate));
}

void RemoteManager::onCreate()
{
  app_hwnd_ = MessageSink::find(L"PowerWinApp", nullptr);
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
  RemoteManager mgr;

  Windows::Application::processMessages();
}

} // namespace PowerWin
