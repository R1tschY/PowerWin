#pragma once

#include <lightports/user/hook.h>
#include <app/module.h>
#include <app/helper/observermixin.h>

constexpr int SHIFTED = 0x8000;

class ScrollPlugin : public PowerWin::Module, PowerWin::ObserverMixin
{
public:
  ScrollPlugin(PowerWin::ModuleContext& context);

private:
  bool inverse_;

  bool handle(POINT pt, int steps, DWORD time);
};

