#pragma once

#include <lightports/extra/hook.h>
#include <app/module.h>

constexpr int SHIFTED = 0x8000;

class ScrollPlugin : public PowerWin::Module {
public:
  ScrollPlugin();

  void activate(PowerWin::ModuleContext& context) override;
  void deactivate() override;

private:
  Windows::MouseWheelHook hook_;
  bool inverse_;

  bool handle(POINT pt, int steps);
};

