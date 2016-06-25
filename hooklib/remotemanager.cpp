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

RemoteManager::RemoteManager()
: mailslot_(L"\\\\.\\mailslot\\PowerWin\\" CPP_WSTRINGIFY(CPUBITSET), 1024, MAILSLOT_WAIT_FOREVER)
{
  print(L"add function: %s len: %u", std::string("SystemMenuHook::activate").c_str(), (unsigned)std::string("SystemMenuHook::activate").size());

  cpp::transform(PowerWin::HookModuleRegistry::entries(), modules_, [](auto& entry){ return entry.create(); });

  mailslot_.registerFunction("activate", [=](auto&){ this->activate(); });
  mailslot_.registerFunction("deactivate", [=](auto&){ this->deactivate(); });
  mailslot_.registerFunction("quit", [=](auto&){ this->quit(); });
}

void RemoteManager::run()
{
  mailslot_.readLoop();
}

void RemoteManager::activate()
{
  cpp::for_each(modules_, [](auto& module) { module->activate(); });
}

void RemoteManager::deactivate()
{
  cpp::for_each(modules_, [](auto& module) { module->deactivate(); });
}

void RemoteManager::quit()
{
  print(L"%s: RemoteManager::quit", POWERWIN_APP_NAME);
  mailslot_.quit();
}

extern "C"
void CALLBACK EnterGodModus(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
  RemoteManager mgr;
  mgr.run();
}
