/*
 * hookmanager.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_REMOTEMANAGER_H_
#define HOOKLIB_REMOTEMANAGER_H_

#include <memory>
#include <vector>
#include <lightports/controls/messagesink.h>

#include "macros.h"
#include "hookmodule.h"

namespace PowerWin {

class RemoteManager : public Windows::MessageSink {
public:
  RemoteManager();

  void activate();
  void deactivate();

  void onCreate() override;
  void onDestroy() override;
  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
  std::vector<std::unique_ptr<HookModule>> modules_;
  HWND app_hwnd_ = nullptr;
  bool activated_ = false;
};

extern "C" {

DLL_PUBLIC
void CALLBACK EnterGodModus(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow);

} // extern "C"

} // namespace PowerWin

#endif /* HOOKLIB_REMOTEMANAGER_H_ */
