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
  struct MenuId {
  enum : unsigned {
    AlwaysOnTop = 0x00270,
  };
  };

  SystemMenuHook(PowerWin::HookModuleContext& context);
  ~SystemMenuHook();

private:
  cpp::scoped_lifetime<Windows::Hook> hook_;
};

#endif /* HOOKLIB_SYSTEMMENUHOOK_H_ */
