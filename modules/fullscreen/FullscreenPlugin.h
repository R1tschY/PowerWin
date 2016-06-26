#pragma once

#include <app/plugin.h>
#include <lightports/extra/hook.h>
#include <lightports/base/timeout.h>

class FullscreenPlugin : public Plugin {
public:
  FullscreenPlugin();

  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

private:
  Windows::Timeout timeout_;
  bool fullscreen_window_;

  void update();
};

