/*
 * hookmanager.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_REMOTEMANAGER_H_
#define HOOKLIB_REMOTEMANAGER_H_

#include "windows/ipc/ipcmailbox.h"

namespace Windows {

class RemoteManager {
public:
  RemoteManager();

  void run();
  void quit();

private:
  IPCMailbox mailslot_;
};

} /* namespace Windows */

extern "C"
void CALLBACK EnterGodModus(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow);

#endif /* HOOKLIB_REMOTEMANAGER_H_ */
