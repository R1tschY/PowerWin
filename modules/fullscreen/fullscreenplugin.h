#pragma once

#include <lightports/extra/hook.h>
#include <lightports/base/timeout.h>
#include <app/module.h>

class FullscreenPlugin : public PowerWin::Module {
public:
  FullscreenPlugin();

  void activate(PowerWin::ModuleContext& context) override;
  void deactivate() override;

private:
  Windows::Timeout timeout_;
  bool fullscreen_window_;

  void update();
};

