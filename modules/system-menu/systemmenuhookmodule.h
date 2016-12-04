/*
 * systemmenuhook.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_SYSTEMMENUHOOK_H_
#define HOOKLIB_SYSTEMMENUHOOK_H_

#include <cpp-utils/storage/uninitized.h>
#include <lightports/user/hook.h>

#include <hooklib/hookmodule.h>

class SystemMenuHook
{
public:
  enum {
    AliveTag = 0x12345678,
    DeadTag = 0xDEADDEAD
  };

  SystemMenuHook(PowerWin::HookModuleContext& context);
  ~SystemMenuHook();

  void onNewWindow(HWND hwnd);
  void onToogleTopmost(HWND hwnd);

  static LRESULT CALLBACK onHookMessage(int code, WPARAM wParam, LPARAM lParam);

  bool isAlive() const { return alive_ == AliveTag; }

private:
  std::uint32_t alive_;
  UINT new_window_msg_;
  UINT set_topmost_msg_;
  HWND powerwin_hwnd_;
  Windows::Hook hook_;
};

class SystemMenuHookModule : public PowerWin::HookModule
{
public:
  SystemMenuHookModule(PowerWin::HookModuleContext& context);

private:
  cpp::scoped_lifetime<SystemMenuHook> hook_;
};

#endif /* HOOKLIB_SYSTEMMENUHOOK_H_ */
