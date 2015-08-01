#include "SystemMenuPlugin.h"

#include <vector>
#include <cstdio>

#include <windows/core.h>
#include <windows/extra/hook.h>
#include <windows/base/application.h>
#include <windows/ipc/ipcdata.h>
#include <c++/utils.h>
#include <c++/macros.h>
#include <c++/uninitized.h>
#include "../macros.h"

SystemMenuPlugin::SystemMenuPlugin() :
  Plugin(L"system_menu")
{
  connection32_ = Windows::IPCConnection(L"\\\\.\\mailslot\\PowerWin\\" WSTRINGIFY(CPUBITSET));
  if (Windows::Application::Is64BitWindows()) { // TODO: HACK: try/catch
    connection64_ = Windows::IPCConnection(L"\\\\.\\mailslot\\PowerWin\\64");
  }
}

void SystemMenuPlugin::onActivate(const Plugin::Options& options)
{
  connection32_.callFunction("SystemMenuHook::activate", Windows::IPCData());
  if (Windows::Application::Is64BitWindows()) { // TODO: HACK: try/catch
    connection64_.callFunction("SystemMenuHook::activate", Windows::IPCData());
  }
}

void SystemMenuPlugin::onDeactivate()
{
  connection32_.callFunction("SystemMenuHook::deactivate", Windows::IPCData());
  connection32_.callFunction("quit", Windows::IPCData()); // TODO

  if (Windows::Application::Is64BitWindows()) { // TODO: HACK: try/catch
    connection64_.callFunction("SystemMenuHook::deactivate", Windows::IPCData());
    connection64_.callFunction("quit", Windows::IPCData()); // TODO
  }
}
