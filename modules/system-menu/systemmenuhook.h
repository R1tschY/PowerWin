/*
 * systemmenuhook.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_SYSTEMMENUHOOK_H_
#define HOOKLIB_SYSTEMMENUHOOK_H_

#include <cpp-utils/storage/uninitized.h>
#include <lightports/extra/hook.h>

#include <hooklib/hookmodule.h>

class SystemMenuHook : public PowerWin::HookModule
{
public:
  SystemMenuHook(PowerWin::HookModuleContext& context);
  ~SystemMenuHook();

  void onNewWindow(HWND hwnd);
  void onToogleTopmost(HWND hwnd);

private:
  cpp::scoped_lifetime<Windows::Hook> hook_;

  UINT new_window_msg_;
  UINT set_topmost_msg_;
  HWND powerwin_hwnd_;
};

#endif /* HOOKLIB_SYSTEMMENUHOOK_H_ */
