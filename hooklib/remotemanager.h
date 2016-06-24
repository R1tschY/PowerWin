/*
 * hookmanager.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_REMOTEMANAGER_H_
#define HOOKLIB_REMOTEMANAGER_H_

#include <lightports/ipc/ipcmailbox.h>
#include "macros.h"

class RemoteManager {
public:
  RemoteManager();

  void run();
  void quit();

private:
  Windows::IPCMailbox mailslot_;
};

extern "C" {

DLL_PUBLIC
void CALLBACK EnterGodModus(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow);

} // extern "C"

#endif /* HOOKLIB_REMOTEMANAGER_H_ */
