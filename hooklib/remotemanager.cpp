/*
 * hookmanager.cpp
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#include "remotemanager.h"

#include <functional>
#include <thread>
#include "c++/macros.h"
#include "macros.h"
#include "systemmenuhook.h"

namespace Windows {

RemoteManager::RemoteManager()
: mailslot_(L"\\\\.\\mailslot\\PowerWin\\" WSTRINGIFY(CPUBITSET), 1024, MAILSLOT_WAIT_FOREVER)
{
  mailslot_.registerFunction("SystemMenuHook::activate", &SystemMenuHook::activate);
  mailslot_.registerFunction("quit", std::bind(&RemoteManager::quit, this));
}

void RemoteManager::run()
{
  mailslot_.readLoop();
}

void RemoteManager::quit()
{
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

} /* namespace Windows */
