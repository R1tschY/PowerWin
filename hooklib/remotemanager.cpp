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
#include "macros.h"
#include "systemmenuhook.h"
#include <lightports/core/debug.h>

RemoteManager::RemoteManager()
: mailslot_(L"\\\\.\\mailslot\\PowerWin\\" CPP_WSTRINGIFY(CPUBITSET), 1024, MAILSLOT_WAIT_FOREVER)
{
  print(L"add function: %s len: %u", std::string("SystemMenuHook::activate").c_str(), (unsigned)std::string("SystemMenuHook::activate").size());

  mailslot_.registerFunction("SystemMenuHook::activate", &SystemMenuHook::activate);
  mailslot_.registerFunction("SystemMenuHook::deactivate", &SystemMenuHook::deactivate);
  mailslot_.registerFunction("quit", std::bind(&RemoteManager::quit, this));
}

void RemoteManager::run()
{
  mailslot_.readLoop();
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
